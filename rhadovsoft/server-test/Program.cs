using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

namespace server_test
{
    class Program
    {
        static void Main(string[] args)
        {
            
            // create and initialize tcp server on port 8080
            IPAddress ip = Dns.GetHostEntry("localhost").AddressList[0];
            TcpListener server = new TcpListener(ip, 1337);
            TcpClient client = default(TcpClient);

            try
            {   
                server.Start();
                Console.WriteLine("Server started... ");
            } catch (Exception ex)
            {
                Console.WriteLine(ex.ToString()+"\nServer start failed!");
                Console.Read();
            }
            client = server.AcceptTcpClient();
            bool quit = false;
            while (quit == false){

                byte[] receiveBuffer = new byte[100];
                NetworkStream stream = client.GetStream();

                stream.Read(receiveBuffer, 0, receiveBuffer.Length);

                string msg = Encoding.ASCII.GetString(receiveBuffer, 0, receiveBuffer.Length);

                Console.WriteLine(msg);
                if (msg == "quit")
                {
                    quit = true;
                    stream.Close();
                    client.Close();
                    server.Stop();
                }
            }
        }
    }
}
