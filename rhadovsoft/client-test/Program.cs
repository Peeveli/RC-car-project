using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

namespace client_test
{
    class Program
    {
        static void Main(string[] args)
        {
            string serverIP = "localhost";
            int port = 1337;

            NetworkStream stream;
            TcpClient client;

            try
            {
                client = new TcpClient(serverIP, port);
                while (true)
                {
                    Console.WriteLine("Send bytes:");
                    string message = Console.ReadLine();
                    int byteCount = Encoding.ASCII.GetByteCount(message);

                    byte[] sendData = new byte[byteCount];

                    sendData = Encoding.ASCII.GetBytes(message);

                    stream = client.GetStream();
                    stream.Write(sendData, 0, sendData.Length);
                }
                stream.Close();
                client.Close();
            }
            catch
            {
                Exception e;
                Console.WriteLine("Connection failed");
            }
        }
    }
}
