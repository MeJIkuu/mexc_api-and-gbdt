#include <fstream>
#include <strstream>
#include <unistd.h>
#include <filesystem>

#include "flags/flags.h"
#include "gbdt/x.h"
#include "gbdt/gbdt.h"
#include "mexc/mexc.hpp"

std::vector<double> SMA(std::vector<double> &data, int period) {
    std::vector<double> sma(data.size(), 0.0);
    double sum = 0;
    for (int i = 0; i < period; i++) {
        sum += data[i];
    }
    sma[period - 1] = sum / period;
    for (int i = period; i < data.size(); i++) {
        sum -= data[i - period];
        sum += data[i];
        sma[i] = sum / period;
    }
    return sma;
}

int main(int argc, char** argv) {
    const flags::args args(argc, argv);

    std::filesystem::create_directory("data");

    TreeParam param;

    param.verbose = 1;
    param.max_level = 5;
    param.max_leaf_number = 20;
    param.min_values_in_leaf = 10;
    param.tree_number = 1000;
    param.learning_rate = 0.01;
    param.training_sample = "train.txt";
    param.training_sample_format = "liblinear";
    param.model = "train.json";
    param.gbdt_sample_rate = 0.9;
    param.gbdt_loss = "ls";

    const auto symbol = args.get<std::string>("symbol");
    if (!symbol) {
        std::cerr << "No symbol" << std::endl;
        return 1;
    }

    const auto period = args.get<std::string>("period");
    if (!period) {
        std::cerr << "No period" << std::endl;
        return 1;
    }

    const auto verbose = args.get<int>("verbose");
    if (!verbose) param.verbose = 1;
    else param.verbose = verbose.value();

    const auto max_level = args.get<size_t>("max_level");
    if (!max_level) param.max_level = 5;
    else param.max_level = max_level.value();

    const auto max_leaf_number = args.get<size_t>("max_leaf_number");
    if (!max_leaf_number) param.max_leaf_number = 20;
    else param.max_leaf_number = max_leaf_number.value();

    const auto min_values_in_leaf = args.get<size_t>("min_values_in_leaf");
    if (!min_values_in_leaf) param.min_values_in_leaf = 10;
    else param.min_values_in_leaf = min_values_in_leaf.value();

    const auto tree_number = args.get<size_t>("tree_number");
    if (!tree_number) param.tree_number = 1000;
    else param.tree_number = tree_number.value();

    const auto learning_rate = args.get<double>("learning_rate");
    if (!learning_rate) param.learning_rate = 0.1;
    else param.learning_rate = learning_rate.value();

    std::strstream training_sample;
    training_sample << "data/" << symbol.value().c_str() << "_" << period.value().c_str() << "_train.dat" << std::ends;

    std::strstream model;
    model << "data/" << symbol.value().c_str() << "_" << period.value().c_str() << "_model.dat" << std::ends;

    param.training_sample = training_sample.str();
    param.model = model.str();

    MexcApi c("", "");

    int input = 10;
    int inputSma = 15;
    int max = 25;

    const auto train = args.get<bool>("train");
    if (train) {
        auto candles = c.getKlines(symbol.value(), period.value(), 0, 0, 1000);
        if (c.error() != 0) {
            std::cout << c.errorString() << std::endl;
            return 2;
        }

        std::vector<double> closes;

        for (auto &item : candles)
            closes.push_back(item.close);

        auto sma = SMA(closes, inputSma);

        std::string in;

        std::ofstream data_file(training_sample.str());

        for (int i = (input + inputSma); i < closes.size() - max; i++)
        {
            in.clear();

            auto h = i + rand() % max;

            if (closes[h] > closes[i])
            {
                in += "+1";
            }
            else
            {
                if (closes[h] < closes[i])
                {
                    in += "-1";
                }
                else
                {
                    in += "-1";
                }
            }

            in += " ";

            in += "1:";
            in += std::to_string(closes[i]);

            for (int j = 0; j < input; j++)
            {
                in += " ";

                in += std::to_string(j+2);
                in += ":";
                in += std::to_string(sma[i - j]);
            }

            data_file << in << "\n";
        }

        data_file.close();

        XYSet set;
        if (param.training_sample_format == "liblinear")
        {
            if (load_liblinear(param.training_sample.c_str(), &set) == -1)
                return 2;
        }
        else
        {
            if (load_gbdt(param.training_sample.c_str(), &set) == -1)
                return 2;
        }

        GBDTTrainer trainer(set, param);
        trainer.train();

        FILE * output = xfopen(param.model.c_str(), "w");
        trainer.save_json(output);
        fclose(output);

        GBDTPredictor predictor;
        FILE * input1 = xfopen(param.model.c_str(), "r");
        predictor.load_json(input1);
        fclose(input1);

        for (size_t i=0, s=set.size(); i<s; i++)
        {
            const XY& xy = set.get(i);
            const CompoundValueVector& X = xy.X();
            double y = xy.y();
            printf("%lf should be near to %lf\n", predictor.predict(X), y);
        }
    }
    else {
        GBDTPredictor predictor;
        FILE * input1 = xfopen(param.model.c_str(), "r");
        predictor.load_json(input1);
        fclose(input1);

        std::string idPos = "";
        int64_t lastBar = 0;

        while (true) {
            auto bars = c.getKlines(symbol.value(), period.value(), 0, 0, 50);
            if(bars.back().open_time != lastBar) {
                lastBar = bars.back().open_time;
                std::vector<double> closes;

                for (auto &kline : bars)
                    closes.push_back(kline.close);

                auto sma = SMA(closes, inputSma);

                std::string in1;
                in1 += "+1 1:";
                in1 += std::to_string(closes.back());

                for (int j = 0; j < input; j++) {
                    in1 += " ";
                    in1 += std::to_string(j+2);
                    in1 += ":";
                    in1 += std::to_string(sma[closes.size() - j - 1]);
                }

                std::ofstream data_file1(param.training_sample.c_str());
                data_file1 << in1;
                data_file1.close();

                XYSet set1;
                if (param.training_sample_format == "liblinear")
                {
                    if (load_liblinear(param.training_sample.c_str(), &set1) == -1)
                        return 2;
                }
                else
                {
                    if (load_gbdt(param.training_sample.c_str(), &set1) == -1)
                        return 2;
                }

                for (size_t i=0, s=set1.size(); i<s; i++)
                {
                    const XY& xy = set1.get(i);
                    const CompoundValueVector& X = xy.X();
                    double y = xy.y();
                    double res = predictor.predict(X);
                    printf("%lf should be near to %lf\n", res, y);

                    if (res > 0.5) {
                        if (idPos != "" ) c.cancelOrder(symbol.value().c_str(), idPos);
                        std::cout << "BUY" << std::endl;
                        auto r = c.sendOrder(symbol.value().c_str(), "BUY", "MARKET", 1.0);
                        if (c.error() == 0) {
                            idPos = r.orderId;
                        }
                        else std::cout << c.errorString() << std::endl;
                    }
                    else if (res < -0.5) {
                        if (idPos != "" ) c.cancelOrder(symbol.value().c_str(), idPos);
                        std::cout << "SELL" << std::endl;
                        auto r = c.sendOrder(symbol.value().c_str(), "SELL", "MARKET", 1.0);
                        if (c.error() == 0) {
                            idPos = r.orderId;
                        }
                        else std::cout << c.errorString() << std::endl;
                    }
                }
            }
            sleep(30);
        }
    }

    return 0;
}
