/*
 * ============================================================================
 * Name:         Program.cs
 * Part of:      RemoteCar project
 * Description:  xinput test
 * Version:      1.0 (test)
 * Author(s):    rhadov
 * License:      No License
 * ============================================================================
 */

using System;
using System.Threading;
using System.Threading.Tasks;
using SharpDX.XInput;

namespace xinput_test
{
    class Program
    {
        public static bool quit = false;
        static void Main(string[] args)
        {
            Console.Clear();
            //Main loop starts
            while (quit == false)
            {
                if (DualShock.CheckController()) //Start checking the controller
                {
                    Task.Factory.StartNew(() => DualShock.ReadControls()); //Start updating the stick information
                    Task.Factory.StartNew(() => DualShock.PrintXY()); //Thread for testing purposes to see what values the controller reads
                    while (quit == false)
                    {
                        if(int.TryParse(Console.ReadLine(), out int num)) //Press any number and hit enter, the program quits.
                        {
                            quit = true;
                        }
                    }
                }
            }
        }
    }
    public class DualShock
    {
        public static Controller controller;
        private static State stateNew;
        public struct controlInfo {
            public int leftStick;
            public int rightStick;
        }
        public static controlInfo ds4;
        public static bool CheckController()
        {
            bool cStatus;
            controller = new Controller(UserIndex.One);
            if (!controller.IsConnected)
            {
                Console.WriteLine("No controller found!");
                Console.ReadKey();
                cStatus = false;
            }
            else
            {
                Console.WriteLine("Controller found!");
                Console.ReadKey();
                cStatus = true;
            }
            return cStatus;
        }
        public static void ReadControls()
        {
            do
            {
                stateNew = controller.GetState();
                ds4.leftStick = stateNew.Gamepad.LeftThumbY;
                ds4.rightStick = stateNew.Gamepad.RightThumbX;
            }
            while (Program.quit == false);
        }
        public static void PrintXY()
        {
            do
            {
                Console.Clear();
                Console.WriteLine("Y: "+ds4.leftStick);
                Console.WriteLine("X: "+ds4.rightStick);
                Thread.Sleep(50);
            }
            while (Program.quit == false);
        }
    }
}
