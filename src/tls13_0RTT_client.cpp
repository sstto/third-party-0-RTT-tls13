#include<iostream>
#include"util/option.h"
#include"util/log.h"
#include"tcpip/server.h"
#include"tls13.h"
using namespace std;



class TLS_client_reduce : public Client
{
public:
    TLS_client_reduce(string ip, int port,string s, struct H e) : Client{ip, port} {
        first_msg = s;
        early_data = e;
        t.handshake_reduce(bind(&TLS_client_reduce::recv, this, 0),
                    bind(&TLS_client_reduce::send, this, placeholders::_1, 0), s, e);
    }
    void encodeNsend(string s) {
        send(t.encode(move(s)));
    }
    optional<string> recvNdecode() {
        return t.decode(*recv());
    }
private:
    TLS13<CLIENT> t;
    string first_msg;
    struct H early_data;
    int get_full_length(const string &s) {
        return s.size() < 5 ? 0 : static_cast<unsigned char>(s[3]) * 0x100
                                  + static_cast<unsigned char>(s[4]) + 5;
    }
};


int main(int ac, char **av) {
    TLS13<SERVER> tmp;
    tmp.set_prv_key_(server_prv_);
    early.server_keyshare = tmp.server_keyshare();
    early.server_certificate = tmp.server_certificate13();
    early.server_certificate_verify = tmp.certificate_verify_reduce();
//        ================================debug==========================================
//    std::cout << std::hex<< std::setw(2) << std::setfill('0') << bnd2mpz((uint8_t*)(&early.server_keyshare[5]),(uint8_t*)(&early.server_keyshare[60])) << std::endl;

    CMDoption co{
            {"port", "port of the host", 4433},
            {"ip", "ip address of the host", "localhost"}
    };
    if(!co.args(ac, av)) return 0;

    TLS_client_reduce t{co.get<const char*>("ip"), co.get<int>("port"),"GET /", early};

//    sending additional application data
//    for(int i=0; i<10000; i++) {
//        t.encodeNsend("GET /");
//        cout << "sending~" << endl;
//        cout << *t.recvNdecode() << endl;}
}

