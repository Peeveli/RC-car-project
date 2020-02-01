/*
 * ============================================================================
 * Name:         Program.cs
 * Part of:      RemoteCar project
 * Description:  client app
 * Version:      1.0 (absolute hell)
 * Author(s):    rhadov
 * License:      No License
 * ============================================================================
 */

// Final conclusion
// Xinput values will be calculated in the same thread where they are read and written.
// They will be constructed into a one string with spaces between the values.
// It will make the splitting between the sent values possible in python script
// Python will 'import re' and use 're.findall(r"[\w']+", string)' to write down all the values in the same order they were sent to.

using System;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Net.Sockets;
using SharpDX.XInput;


namespace client.cs
{
    class Program
    {
        public static bool quit = false;
        public static string serverIP = ""; //Set target ip address
        public static int port = 1337; // set target port
        static void Main(string[] args)
        {
            while (quit == false)
            {
                Console.Clear();
                Console.WriteLine("1: Drive with controller | 2: Drive with keyboard | 3: quit\n");
                int item;
                bool conversion = int.TryParse(Console.ReadKey().KeyChar.ToString(), out item);

                if (conversion == true)
                {
                    switch (item)
                    {
                        case 1:
                            Drive(0);
                            quit = false;
                            break;
                        case 2:
                            Drive(1);
                            quit = false;
                            break;
                        case 3:
                            quit = true;
                            break;
                    }
                }
            }
        }
        static void Drive(int mode)
        {
            NetworkStream stream;
            TcpClient client;
            Console.Clear();
            Console.WriteLine("TARGET PORT: ");
            if (int.TryParse(Console.ReadLine().ToString(), out port))
            {
                Console.WriteLine("\nTARGET IP ADDRESS (x.x.x.x): ");
                serverIP = Console.ReadLine();
            }
            else
            {
                Console.WriteLine("\nThats not a valid port...");
            }
            switch (mode)
            {

                case 0:
                    if (DualShock.CheckController()) //Start checking the controller
                    {
                        Console.WriteLine("Controller found!");
                        Task.Factory.StartNew(() => DualShock.ReadControls()); //Start updating the stick information
                        //Task.Factory.StartNew(() => DualShock.PrintXY()); //Thread for testing purposes to see what values the controller reads
                    } else
                    {
                        Console.WriteLine("No controller found! Press any key to to return");
                        Console.ReadKey();
                        quit = true;
                    }
                    break;
                case 1:
                    Task.Factory.StartNew(() => ReadKeyDrive()); // Start reading keyboardinput
                    break;
            }
            // try to open TCP connection to target
            try
            {
                client = new TcpClient(serverIP, port);
                while (quit == false)
                {
                    // get the message string from dualshock class
                    string message = DualShock.ds4.message;
                    // check that it is not empty
                    if (string.IsNullOrEmpty(message) != true)
                    {
                        // send the data to the target
                        int byteCount = Encoding.ASCII.GetByteCount(message);
                        byte[] sendData = new byte[byteCount];
                        sendData = Encoding.ASCII.GetBytes(message);
                        stream = client.GetStream();
                        stream.Write(sendData, 0, sendData.Length);
                        if (quit == true)
                        {
                            stream.Close();
                        }
                        // A bit delaying to not to flood our target
                        Thread.Sleep(50);
                    }
                }
                client.Close();
            }
            catch
            {
                Exception e;
                Console.WriteLine("\nConnection failed! Press any key to retry");
                quit = true;
                Console.ReadKey();
            }
        }
        static void ReadKeyDrive() {
            do
            {
                Console.Clear();
                Console.WriteLine("1: DRIVE\n2: REVERSE\n3: LEFT\n4: RIGHT\n5: QUIT");
                int item;
                bool conversion = int.TryParse(Console.ReadKey().KeyChar.ToString(), out item);
                switch (item)
                {
                    case 1:
                        DualShock.ds4.message = "drive";
                        break;
                    case 2:
                        DualShock.ds4.message = "reverse";
                        break;
                    case 3:
                        DualShock.ds4.message = "left";
                        break;
                    case 4:
                        DualShock.ds4.message = "right";
                        break;
                    case 5:
                        DualShock.ds4.message = "quit";
                        quit = true;

                        break;
                }
            } while (quit == false);

            Thread.Sleep(1000);
            DualShock.ds4.message = "";
        }
    }

    public class DualShock
    {
        public static Controller controller;
        public static int div = 18;
        private static State stateNew;
        public struct controlInfo
        {
            public int leftStick;
            public int rightStick;
            public string dir;
            public string speed;
            public string lights;
            public string turn;
            public string message;
        }
        public static controlInfo ds4;
        public static bool CheckController()
        {
            // check if there is controller 
            bool cStatus;
            controller = new Controller(UserIndex.One);
            if (!controller.IsConnected)
            {
                cStatus = false;
            }
            else
            {
                cStatus = true;
            }
            return cStatus;
        }
        public static void ReadControls()
        {
            Console.Clear();
            Console.WriteLine("Press ANY key to quit.");
            do
            {
                stateNew = controller.GetState();
                // READ CONTROLLER RAW INPUT
                ds4.leftStick = stateNew.Gamepad.LeftThumbY; //32767 --> forward
                ds4.rightStick = stateNew.Gamepad.RightThumbX; //32767 --> right
                // Calculates the input and creates the string with logic

                // driving
                if (ds4.leftStick < -5000)
                {
                    //going forward
                    ds4.dir = "2 ";
                    ds4.speed = ((ds4.leftStick / 18)*-1).ToString()+" ";
                    
                } else if (ds4.leftStick > 5000)
                {
                    // going reverse
                    ds4.dir = "1 ";
                    ds4.speed = (ds4.leftStick / 18).ToString()+" ";

                } else
                {
                    // doing idle
                    ds4.dir = "0 ";
                    ds4.speed = "0 ";
                }
                // turning
                if (ds4.rightStick < -5000)
                {
                    // turning left
                    ds4.turn = (150-((ds4.rightStick / 110*-1)/2)).ToString(); // outputs something between 150-0
                    
                } else if (ds4.rightStick > 5000)
                {
                    // turning right
                    ds4.turn = (((ds4.rightStick / 110) / 2)+150).ToString(); // outputs something between 150-290
                } else
                {
                    //ds4.turndir = "0 ";
                    ds4.turn = "150 ";
                }

                // generate string
                ds4.message = "";
                ds4.message = ds4.dir + ds4.speed + ds4.turn;
                if (Console.KeyAvailable == true)
                {
                    Program.quit = true;
                }
            }
            while (Program.quit == false);
        }
        //This is just for testing purpose to see what we are sending
        public static void PrintXY()
        {
            do
            {
                Console.Clear();
                Console.WriteLine(ds4.message);
                Thread.Sleep(100);
            }
            while (Program.quit == false);
        }
    }
}
