#include "http_connection.hpp"
#include "Storage.hpp"

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <thread>
#include <vector>

using namespace service_node;

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments.
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80\n";
            return EXIT_FAILURE;
        }
        std::cout << "Listening at address " << argv[1] << " port "
                  << argv[2] << std::endl;

        auto const address = boost::asio::ip::make_address(argv[1]);
        unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

        boost::asio::io_context ioc{1};

        Storage storage(".");

        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};
        http_server(acceptor, socket, storage);

        ioc.run();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
