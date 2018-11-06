#include "keriocurlcore.h"

KerioCurlCore::KerioCurlCore(QObject *parent) : QObject(parent)
{
    jRpcEngine = new JsonRpcEngine();
}

KerioCurlCore::~KerioCurlCore()
{

}

bool KerioCurlCore::setUrl(std::string host, int port, std::string target)
{
    //bool check = false;
    global_url = "https://" + host + ":" + std::to_string(port) + target;
    /*CURL* checkHost;
    CURLcode code;
    curl_global_init(CURL_GLOBAL_ALL);
    checkHost = curl_easy_init();
    if (checkHost)
    {
        curl_easy_setopt(checkHost, CURLOPT_URL, global_url.c_str());
        curl_easy_setopt(checkHost, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(checkHost, CURLOPT_SSL_VERIFYHOST, 0L);
        code = curl_easy_perform(checkHost);
        if (code == CURLE_OK)
        {
            check = true;
        } else {
            check = false;
        }
    } else {
        check = false;
    }
    curl_easy_cleanup(checkHost);
    curl_global_cleanup();
    checkHost = NULL;*/
    return true;
}

QHash<QString, QVariant> KerioCurlCore::perform()
{
    QFile file("cookies.txt");
    file.open(QIODevice::ReadWrite);
    std::cout << "TOKEN :" << x_token << std::endl;
    CURLcode perf;
    QHash<QString, QVariant> response;
    curl_global_init(CURL_GLOBAL_ALL);
    std::string ex_data;
    ex_data.clear();
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Accept: application/json-rpc");
    headers = curl_slist_append(headers, "Content-Type: application/json-rpc");
    headers = curl_slist_append(headers, "User-Agent: King Anthony");
    headers = curl_slist_append(headers, "Host: ats.550550.ru:4021");
    headers = curl_slist_append(headers, "Connection: close");


    if (x_token != "")
    {

        std::string _session = "Cookie: ";
        struct curl_slist* cookies = nullptr;
        std::string _token = "X-Token: "+x_token;
        headers = curl_slist_append(headers, _token.c_str());


        QTextStream out(&file);
        while (!out.atEnd())
        {
            QString line = out.readLine();
            _session = line.toStdString();
            cookies = curl_slist_append(cookies, _session.c_str());
        }
    } else {
    }
    curlCore = curl_easy_init();
    if (curlCore)
    {
        curl_easy_setopt(curlCore, CURLOPT_URL, global_url.c_str());
        curl_easy_setopt(curlCore, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curlCore, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlCore, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curlCore, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curlCore, CURLOPT_COOKIEFILE, "cookies.txt");
        curl_easy_setopt(curlCore, CURLOPT_COOKIEJAR, "cookies.txt");
        curl_easy_setopt(curlCore, CURLOPT_POST, 1L);
        curl_easy_setopt(curlCore, CURLOPT_POSTFIELDSIZE, post_request.size());
        curl_easy_setopt(curlCore, CURLOPT_POSTFIELDS, post_request.c_str());
        curl_easy_setopt(curlCore, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curlCore, CURLOPT_WRITEDATA, &ex_data);
        //curl_easy_setopt(curlCore, CURLOPT_VERBOSE, 1L);
        perf = curl_easy_perform(curlCore);
        if (perf == CURLE_OK)
        {
            //std::cout << ex_data << std::endl;
            response = jRpcEngine->parse(QString::fromStdString(ex_data));
            read_cookies(curlCore);
            curl_easy_cleanup(curlCore);
        }
    }
    //file.close();
    curl_slist_free_all(headers);
    curl_global_cleanup();
    return response;
}

size_t KerioCurlCore::writer(char* contents, size_t nmemb, size_t size, std::string *data)
{
    data->append(contents, nmemb * size);
    return nmemb * size;
}

bool KerioCurlCore::setAppInfo(QString app, QString vendor, QString rev)
{
    jRpcEngine->setAppInfo(app, vendor, rev);
    return true;
}

bool KerioCurlCore::prepare(KerioMethod method, QHash<KerioParams, QVariant> params)
{
    bool check = false;
    _method_ = method;
    x_token = params[Token].toString().toStdString();
    if (method == LogIn)
    {
        jRpcEngine->removeToken();
    }
    jRpcEngine->setToken(QString::fromStdString(x_token));
    bool a = jRpcEngine->method(method);
    bool b = jRpcEngine->setParameter(params);
    if (a && b)
    {
        post_request = jRpcEngine->generate();

        //std::cout << post_request.c_str() << std::endl;
        //std::cout << post_request.size() << "post request size" << std::endl;
        check = true;
    } else {
        check = false;
    }
    return check;
}

bool KerioCurlCore::read_cookies(CURL* curl)
{
    auto check = false;
    CURLcode res;


    QFile file("cookies.txt");
    struct curl_slist* cookies;
    struct curl_slist* nc;
    int i;

    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res != CURLE_OK)
    {
        std::cout << curl_easy_strerror(res);
        check = false;
    } else {
        QFile::remove("cookies.txt");
        if (file.open(QIODevice::ReadWrite))
        {
            nc = cookies;
            i = 1;
            QTextStream in(&file);
            while (nc)
            {
                //printf("[%d]: %s\n", i, nc->data);
                in << nc->data;
                in << "\n";
                nc = nc->next;
                i++;
            }
            file.close();
            if (i == 1)
            {
                std::cout << "nothing in cookies!" << std::endl;
            }

            curl_slist_free_all(cookies);

        } else {
            check = false;
        }
    }
    return check;
}





