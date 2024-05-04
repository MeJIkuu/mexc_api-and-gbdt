#ifndef MEXC_HPP
#define MEXC_HPP

#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <sys/time.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <nlohmann/json.hpp>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

#define API_ENDPOINT "https://api.mexc.com"
#define API_KEY_HEADER "X-MEXC-APIKEY"

#define Kline_1m = "1m"
#define Kline_5m = "5m"
#define Kline_15m = "15m"
#define Kline_30m = "30m"
#define Kline_60m = "60m"
#define Kline_4h = "4h"
#define Kline_1d = "1d"
#define Kline_1W = "1M"
#define Kline_1M = "1W"

#define ORDER_BUY = "BUY"
#define ORDER_SELL = "SELL"

#define ORDER_LIMIT = "LIMIT"
#define ORDER_MARKET = "MARKET"
#define ORDER_LIMIT_MAKER = "LIMIT_MARKET"
#define ORDER_IMMEDIATE_OR_CANCEL = "IMMEDIATE_OR_CANCEL"
#define ORDER_FILL_OR_KILL = "FILL_OR_KILL"

struct Header {
    std::string key;
    std::string value;
};

struct Symbol {
    std::string symbol;
    std::string status;
    std::string baseAsset;
    int baseAssetPrecision;
    std::string quoteAsset;
    int quotePrecision;
    int quoteAssetPrecision;
    int baseCommissionPrecision;
    int quoteCommissionPrecision;
    std::vector<std::string> orderTypes;
    bool isSpotTradingAllowed;
    bool isMarginTradingAllowed;
    std::string quoteAmountPrecision;
    std::string baseSizePrecision;
    std::vector<std::string> permissions;
    std::vector<std::string> filters;
    std::string maxQuoteAmount;
    std::string makerCommission;
    std::string takerCommission;
    std::string quoteAmountPrecisionMarket;
    std::string maxQuoteAmountMarket;
    std::string fullName;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Symbol, symbol, status, baseAsset, baseAssetPrecision, quoteAsset, quotePrecision,
                                   quoteAssetPrecision, baseCommissionPrecision, quoteCommissionPrecision, orderTypes, isSpotTradingAllowed, isMarginTradingAllowed,
                                   quoteAmountPrecision, baseSizePrecision, permissions, filters, maxQuoteAmount, makerCommission, takerCommission,
                                   quoteAmountPrecisionMarket, maxQuoteAmountMarket, fullName)
};

struct ExchangeInfo {
    std::string timezone;
    uint64_t serverTime;
    std::vector<std::string> rateLimits;
    std::vector<std::string> exchangeFilters;
    std::vector<Symbol> symbols;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ExchangeInfo, timezone, serverTime, rateLimits, exchangeFilters, symbols)
};

struct DepthPrice {
    int lastUpdateId;
    std::vector<std::vector<std::string>> bids;
    std::vector<std::vector<std::string>> asks;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DepthPrice, lastUpdateId, bids, asks)
};

struct Trade {
    nlohmann::json id;
    std::string price;
    std::string qty;
    std::string quoteQty;
    int64_t time;
    bool isBuyerMaker;
    bool isBestMatch;
    std::string tradeType;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Trade, id, price, price, qty, quoteQty, time, isBuyerMaker, isBestMatch, tradeType)
};

using Trades = std::vector<Trade>;

struct AggTrade {
    nlohmann::json a;
    nlohmann::json f;
    nlohmann::json l;
    std::string p;
    std::string q;
    int64_t T;
    bool m;
    bool M;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AggTrade, a, f, l, p, q, T, m, M)
};

using AggTrades = std::vector<AggTrade>;

struct Kline {
    int64_t open_time;
    double open;
    double high;
    double low;
    double close;
    double volume;
    int64_t close_time;
    double quote_asset_volume;
};
using Klines = std::vector<Kline>;

struct AvgPrice {
    int mins;
    std::string price;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AvgPrice, mins, price)
};

struct s24hr {
    std::string symbol;
    std::string priceChange;
    std::string priceChangePercent;
    std::string prevClosePrice;
    std::string lastPrice;
    std::string bidPrice;
    std::string bidQty;
    std::string askPrice;
    std::string askQty;
    std::string openPrice;
    std::string highPrice;
    std::string lowPrice;
    std::string volume;
    nlohmann::json quoteVolume;
    int64_t openTime;
    int64_t closeTime;
    nlohmann::json count;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(s24hr, symbol, priceChange, priceChangePercent, prevClosePrice, lastPrice, bidPrice, bidQty,
                                   askPrice, askQty, openPrice, highPrice, lowPrice, volume, quoteVolume, openTime, closeTime, count)
};

struct BookTicker {
    std::string symbol;
    std::string bidPrice;
    std::string bidQty;
    std::string askPrice;
    std::string askQty;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookTicker, symbol, bidPrice, bidQty, askPrice, askQty)
};

struct Price {
    std::string symbol;
    std::string price;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Price, symbol, price)
};

struct OrderOpen {
    std::string symbol;
    std::string orderId;
    int64_t orderListId;
    std::string price;
    std::string origQty;
    std::string type;
    std::string side;
    int64_t transactTime;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderOpen, symbol, orderId, orderListId, price, origQty, type, side, transactTime)
};

struct OrderClode {
    std::string symbol;
    std::string origClientOrderId;
    int64_t orderId;
    std::string clientOrderId;
    std::string price;
    std::string origQty;
    std::string executedQty;
    std::string cummulativeQuoteQty;
    std::string status;
    std::string timeInForce;
    std::string type;
    std::string side;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderClode, symbol, origClientOrderId, orderId, clientOrderId, price, origQty, executedQty,
                                   cummulativeQuoteQty, status, timeInForce, type, side)
};

class QueryParams
{
    std::vector<std::string> params;

public:
    QueryParams() {
    };

    void add_new_query(std::string key, std::string value) {
        if (!value.empty())
            params.push_back(key + "=" + value);
    }

    void add_new_query(std::string key, int value) {
        if (value != 0)
            params.push_back(key + "=" + std::to_string(value));
    }

    void add_new_query(std::string key, long value) {
        if (value != 0)
            params.push_back(key + "=" + std::to_string(value));
    }

    void add_new_query(std::string key, uint64_t value) {
        if (value != 0)
            add_new_query(key, std::to_string(value));
    }

    void add_new_query(std::string key, double value) {
        if (value != 0.0)
            add_new_query(key, std::to_string(value));
    }

    void add_new_query(std::string key, bool value) {
        if (value)
            add_new_query(key, "true");
        else
            add_new_query(key, "false");
    }

    void add_new_query(std::string key, std::vector<std::string> value) {
        for (auto &v : value) {
            add_new_query(key, v);
        }
    }

    void add_signature(std::string sig) {
        add_new_query("signature", sig);
    }

    std::string to_str() {
        std::string ret;
        for (auto i = 0; i < params.size(); ++i) {
            ret += params[i];
            if (i != params.size() - 1)
                ret += "&";
        }
        return ret;
    }
};

class MexcApi
{
public:
    MexcApi(std::string api_key, std::string secret_key) :
        api_key_header{API_KEY_HEADER},
        endpoint{API_ENDPOINT} {
        this->api_key = api_key;
        this->secret_key = secret_key;
    }

    ~MexcApi() {
    }

    void setApiKeys(std::string api_key, std::string secret_key) {
        this->api_key = api_key;
        this->secret_key = secret_key;
    }

public:
    enum class Action {
        GET_ACTION,
        POST_ACTION,
        PUT_ACTION,
        DELETE_ACTION
    };

    std::string get_Action(Action action) {
        switch (action) {
        case Action::GET_ACTION:
            return "GET";
        case Action::POST_ACTION:
            return "POST";
        case Action::PUT_ACTION:
            return "PUT";
        case Action::DELETE_ACTION:
            return "DELETE";
        default:
            std::cout << "no such action" << std::endl;
        }
        return "";
    }

public:
    bool ping() {
        std::string url = endpoint + "/api/v3/ping";
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        if (res_curl == CURLE_OK && curl_buffer == "{}") return true;
        else return false;
    }

    uint64_t getTime() {
        std::string url = endpoint + "/api/v3/time";
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer))
                result = nlohmann::json::parse(curl_buffer);
            return result["serverTime"];
        }

        return false;
    }

    std::vector<std::string> getDefaultSymbols() {
        std::string url = endpoint + "/api/v3/defaultSymbols";
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        std::vector<std::string> res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer))
                result = nlohmann::json::parse(curl_buffer);

            if (result["code"].get<int>() == 0)
                res = result["data"].get<std::vector<std::string>>();
        }

        return res;
    }

    ExchangeInfo getExchangeInfo(std::string symbol = "") {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        std::string url = endpoint + "/api/v3/exchangeInfo?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        ExchangeInfo res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<ExchangeInfo>();
            }
        }

        return res;
    }

    DepthPrice getDepth(std::string symbol = "", int limit = 0) {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        query_params.add_new_query("limit", limit);
        std::string url = endpoint + "/api/v3/depth?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        DepthPrice res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<DepthPrice>();
            }
        }

        return res;
    }

    Trades getTrades(std::string symbol = "", int limit = 0) {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        query_params.add_new_query("limit", limit);
        std::string url = endpoint + "/api/v3/trades?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        Trades res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<Trades>();
            }
        }

        return res;
    }

    AggTrades getAggTrades(std::string symbol = "", long startTime = 0, long endTime = 0, int limit = 0) {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        query_params.add_new_query("startTime", startTime);
        query_params.add_new_query("endTime", endTime);
        query_params.add_new_query("limit", limit);
        std::string url = endpoint + "/api/v3/aggTrades?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        AggTrades res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<AggTrades>();
            }
        }

        return res;
    }

    Klines getKlines(std::string symbol = "", std::string interval = "", long startTime = 0, long endTime = 0, int limit = 0) {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        query_params.add_new_query("interval", interval);
        query_params.add_new_query("startTime", startTime);
        query_params.add_new_query("endTime", endTime);
        query_params.add_new_query("limit", limit);
        std::string url = endpoint + "/api/v3/klines?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        Klines res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) {
                    for (int i=0;i<result.size();i++) {
                        Kline item;
                        item.open_time = result[i][0].get<uint64_t>();
                        item.open   = std::stod(result[i][1].get<std::string>());
                        item.high   = std::stod(result[i][2].get<std::string>());
                        item.low    = std::stod(result[i][3].get<std::string>());
                        item.close  = std::stod(result[i][4].get<std::string>());
                        item.volume = std::stod(result[i][5].get<std::string>());
                        item.close_time = result[i][6].get<uint64_t>();
                        item.quote_asset_volume = std::stod(result[i][7].get<std::string>());
                        res.push_back(item);
                    }
                }
            }
        }

        return res;
    }

    AvgPrice getAvgPrice(std::string symbol = "") {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        std::string url = endpoint + "/api/v3/avgPrice?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        AvgPrice res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<AvgPrice>();
            }
        }

        return res;
    }

    s24hr get24hr(std::string symbol = "") {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        std::string url = endpoint + "/api/v3/ticker/24hr?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        s24hr res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<s24hr>();
            }
        }

        return res;
    }

    Price getPrice(std::string symbol = "") {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        std::string url = endpoint + "/api/v3/ticker/price?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        Price res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<Price>();
            }
        }

        return res;
    }

    BookTicker getBookTicker(std::string symbol = "") {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        std::string url = endpoint + "/api/v3/ticker/bookTicker?" + query_params.to_str();
        auto res_curl = startCurl(url, "", headr, Action::GET_ACTION);

        BookTicker res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<BookTicker>();
            }
        }

        return res;
    }

    bool testOrder(std::string symbol = "", std::string side = "BUY", std::string type = "MARKET", double quantity = 0.0,
                   double quoteOrderQty = 0.0, double price = 0.0, std::string newClientOrderId = "", long recvWindow = 0,
                   long timestamp = 0)
    {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        query_params.add_new_query("side", side);
        query_params.add_new_query("type", type);
        if (quantity != 0)
            query_params.add_new_query("quantity", quantity);
        if (quoteOrderQty != 0)
            query_params.add_new_query("quoteOrderQty", quoteOrderQty);
        if (price != 0)
            query_params.add_new_query("price", price);
        if (newClientOrderId != "")
            query_params.add_new_query("newClientOrderId", newClientOrderId);
        if (recvWindow != 0)
            query_params.add_new_query("recvWindow", recvWindow);
        query_params.add_new_query("timestamp", toString(get_current_ms_epoch()));
        query_params.add_signature(hmac_sha256(secret_key.c_str(), query_params.to_str().c_str()));

        std::string url = endpoint + "/api/v3/order/test?";

        std::vector<Header> headr1 = {{"Content-Type", "application/json"},
                                      {api_key_header, api_key}};

        auto res_curl = startCurl(url, query_params.to_str(), headr1, Action::POST_ACTION);

        if (res_curl == CURLE_OK && curl_buffer == "{}")
            return true;
        else
            return false;
    }

    OrderOpen sendOrder(std::string symbol = "", std::string side = "BUY", std::string type = "MARKET", double quantity = 0.0,
                        double quoteOrderQty = 0.0, double price = 0.0, std::string newClientOrderId = "", long recvWindow = 0,
                        long timestamp = 0)
    {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        query_params.add_new_query("side", side);
        query_params.add_new_query("type", type);
        if (quantity != 0)
            query_params.add_new_query("quantity", quantity);
        if (quoteOrderQty != 0)
            query_params.add_new_query("quoteOrderQty", quoteOrderQty);
        if (price != 0)
            query_params.add_new_query("price", price);
        if (newClientOrderId != "")
            query_params.add_new_query("newClientOrderId", newClientOrderId);
        if (recvWindow != 0)
            query_params.add_new_query("recvWindow", recvWindow);
        query_params.add_new_query("timestamp", toString(get_current_ms_epoch()));
        query_params.add_signature(hmac_sha256(secret_key.c_str(), query_params.to_str().c_str()));

        std::string url = endpoint + "/api/v3/order?";

        std::vector<Header> headr1 = {{"Content-Type", "application/json"},
                                      {api_key_header, api_key}};

        auto res_curl = startCurl(url, query_params.to_str(), headr1, Action::POST_ACTION);

        OrderOpen res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<OrderOpen>();
            }
        }

        return res;
    }

    OrderClode cancelOrder(std::string symbol = "", std::string odreId = "", std::string origClientOrderId = "",
                           std::string newClientOrderId = "", long recvWindow = 0, long timestamp = 0)
    {
        QueryParams query_params;
        query_params.add_new_query("symbol", symbol);
        if (odreId != "")
            query_params.add_new_query("odreId", odreId);
        if (origClientOrderId != "")
            query_params.add_new_query("origClientOrderId", origClientOrderId);
        if (newClientOrderId != "")
            query_params.add_new_query("newClientOrderId", newClientOrderId);
        if (recvWindow != 0)
            query_params.add_new_query("recvWindow", recvWindow);
        query_params.add_new_query("timestamp", toString(get_current_ms_epoch()));
        query_params.add_signature(hmac_sha256(secret_key.c_str(), query_params.to_str().c_str()));

        std::string url = endpoint + "/api/v3/order?";

        std::vector<Header> headr1 = {{"Content-Type", "application/json"},
                                      {api_key_header, api_key}};

        auto res_curl = startCurl(url, query_params.to_str(), headr1, Action::DELETE_ACTION);

        OrderClode res;

        if (res_curl == CURLE_OK) {
            nlohmann::json result;
            if (nlohmann::json::accept(curl_buffer)) {
                result = nlohmann::json::parse(curl_buffer);
                parceError(result);
                if (errorCode == 0) res = result.get<OrderClode>();
            }
        }

        return res;
    }

    int error() {
        return errorCode;
    }

    std::string errorString() {
        return errorStr;
    }

private:
    inline void parceError(nlohmann::json result) {
        auto code = result.find("code");
        auto msg = result.find("msg");
        if (code != result.end()) {
            errorCode = code->get<int>();
            if (msg != result.end())
                errorStr = msg->get<std::string>();
        }
    }
    static size_t curl_callback(char *data, size_t size, size_t nmemb, std::string *buffer) {
        size_t result = 0;

        if (buffer != NULL) {
            buffer->append(data, size * nmemb);
            result = size * nmemb;
        }
        return result;
    }

    CURLcode startCurl(std::string url, std::string data, std::vector<Header> headers, Action action) {
        errorCode = 0;
        errorStr = "";

        curl_buffer.clear();

        CURL *curl;
        curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&curl_buffer);

        struct curl_slist *list = NULL;
        for (auto &h : headers) {
            std::string val{h.key + ": " + h.value};
            list = curl_slist_append(list, val.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        if (action == Action::POST_ACTION) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, get_Action(action).c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
        else if (action == Action::PUT_ACTION || action == Action::DELETE_ACTION) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, get_Action(action).c_str());
        }

        CURLcode res;
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            errorCode = res;
            errorStr = curl_easy_strerror(res);
        }

        curl_easy_cleanup(curl);

        return res;
    }

    std::string b2a_hex(char *byte_arr, int n)
    {
        const static std::string HexCodes = "0123456789abcdef";
        std::string HexString;
        for ( int i = 0; i < n ; ++i ) {
            unsigned char BinValue = byte_arr[i];
            HexString += HexCodes[( BinValue >> 4 ) & 0x0F];
            HexString += HexCodes[BinValue & 0x0F];
        }

        return HexString;
    }

    std::string hmac_sha256(const char *key, const char *data)
    {
        unsigned char digest[32];
        mbedtls_md_hmac( mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                        reinterpret_cast<const unsigned char*>(key), strlen(key),
                        reinterpret_cast<const unsigned char*>(data), strlen(data),
                        digest );
        return b2a_hex((char *)digest, 32);
    }

    std::uint64_t get_current_ms_epoch()
    {
        return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                              std::chrono::system_clock::now().time_since_epoch()
                                              ).count());
    }

    template<typename T> std::string toString(const T& val)
    {
        std::ostringstream out;
        out << val;
        return out.str();
    }

private:
    std::string api_key;
    std::string secret_key;
    const std::string api_key_header;
    const std::string endpoint;
    std::string curl_buffer;
    std::vector<Header> headr = {{"Content-Type", "application/json"}};
    int errorCode;
    std::string errorStr;
};

#endif // MEXC_HPP
