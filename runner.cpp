#include "runner.h"

Runner::Runner(Connection conn, File_manager file_manager)
{
    Client client(conn.server, conn.port, conn.encryption);
    if (conn.encryption)
    {
        if (!client.verify_certificate())
        {
            std::cout << "Certificate verification failed" << std::endl;
            exit(1);
        }
    }

    Commands commands = Commands();
    MsgParser parser = MsgParser();

    client.receive(tag);

    client.send(commands.login(tag++, conn.user_name, conn.user_password));
    parser.get_capability(client.receive(tag));
    client.send(commands.select(tag++, conn.inbox));
    std::string response = client.receive(tag);

    parser.get_message_count(response);
    std::cout << "Message count: " << parser.message_count << std::endl;
   
    // get unseen message count
    if(conn.only_new_messages){
        client.send(commands.get_new_messages(tag++));
        response = client.receive(tag);
        std::vector<int> new_messages = parser.get_new_messages(response);

        for (int i = 0; i < new_messages.size(); i++)
        {
            client.send(commands.fetch(tag++, new_messages[i]));
            response = client.receive(tag);
            std::string file_name = parser.get_file_name(response);
            file_manager.save_mail(file_name, response, conn.out_dir);
        }
        
    }

    if (conn.only_message_headers)
    {
        for (int i = 1; i <= parser.message_count; i++)
        {
            client.send(commands.fetch_header(tag++, conn.inbox, i));
            response = client.receive(tag);
            std::string file_name = parser.get_file_name(response);
            file_manager.save_mail(file_name, response, conn.out_dir);
        }
    }
    else
    {
        int start = conn.only_new_messages ? parser.message_count - parser.new_messages + 1 : 1;
        for (int i = start; i <= parser.message_count; i++)
        {
            client.send(commands.fetch_header_important(tag++, conn.inbox, i));
            response = client.receive(tag);
            std::string file_name = parser.get_file_name(response);

            client.send(commands.fetch(tag++, i));
            response = client.receive(tag);
            file_manager.save_mail(file_name, response, conn.out_dir);
        }
    }
    client.send(commands.logout(tag++));
    client.receive(tag);
    client.~Client();
}