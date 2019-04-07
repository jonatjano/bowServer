#include <uWS/uWS.h>
#include <iostream>
#include <list>

bool startsWith(char const* haystack, char const* needle) {
    return strlen(haystack) >= strlen(needle) && strncmp (haystack, needle, strlen(needle)) == 0;
}

class UserData {
private:
    std::string pseudo;

public:
    UserData(std::string pseudo) {
        this->pseudo = pseudo;
    }

    const std::string &getPseudo() const {
        return pseudo;
    }

    void setPseudo(const std::string &pseudo) {
        UserData::pseudo = pseudo;
    }

};

void broadcast(std::list<uWS::WebSocket<uWS::SERVER>*> list, std::string message) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        (*it)->send(message.c_str());
    }
}

int main() {
    std::list<uWS::WebSocket<uWS::SERVER>*> sockets;

    uWS::Hub h;

    h.onMessage([&sockets](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {

        if (startsWith(message, "/pseudo")) {

            if (length < 9) {
                ws->send("Usage: /pseudo <new pseudo>");
                return;
            }

            UserData* user = static_cast<UserData *>(ws->getUserData());

            std::string newPseudo = message;
            newPseudo = (newPseudo).substr(0, length).substr(8);

            std::string oldPseudo = user->getPseudo();

            std::string response = oldPseudo + " is now " + newPseudo;

            broadcast(sockets, response);
            user->setPseudo(newPseudo);
            std::cout << "pseudo changed " << response << std::endl;

        } else if (startsWith(message, "/debug")) {

            std::cout << message << std::endl;

        } else {

            if (length == 0) {
                return;
            }

            UserData* user = static_cast<UserData *>(ws->getUserData());

            std::string response(message);
            response = response.substr(0, length);

            response = user->getPseudo() + " : " + response;
            broadcast(sockets, response);
            std::cout << "message sent " << response << std::endl;

        }

    });

    h.onHttpRequest([&](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length, size_t remainingBytes) {

        std::string response = "Hello!";
        res->end(response.data(), response.length());

    });

    h.onConnection([&sockets](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {

        sockets.push_front(ws);

        const auto MIN = 10000;
        const auto MAX = 99999;
        int guestNumber = MIN + (rand() % static_cast<int>(MAX - MIN + 1));

        std::string guestName = "guest" + std::to_string(guestNumber);

        UserData* ud = new UserData(guestName);

        ws->setUserData((void*)ud);

        std::string personalResponse = "You are connected as " + guestName + " use /pseudo to change your pseudo";
        ws->send(personalResponse.c_str());

        std::string response = "New client connected : " + guestName;
        broadcast(sockets, response);
        std::cout << response << std::endl;

    });

    h.onDisconnection([&sockets](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {

        sockets.remove(ws);

        UserData* user = static_cast<UserData *>(ws->getUserData());

        std::string response = user->getPseudo() + " disconnected";
        broadcast(sockets, response);
        std::cout << response << std::endl;

    });

    if (h.listen(3000)) {
        printf("Opening server on port : %d\n", 3000);
        h.run();
    }


}