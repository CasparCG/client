using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using Settings = Caspar_Pilot.Properties.Settings;
using System.Net;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace Caspar_Pilot.Clients
{
    class SpyderClient
    {
        public static void Send(RundownItem item)
        {
            if (string.IsNullOrEmpty(item.StaticItem.Clipname))
                return;

            UdpClient client = new UdpClient();
            client.Connect(IPAddress.Parse(Settings.Default.SpyderIp), Settings.Default.SpyderPort);

            string command = Settings.Default.SpyderSettings
                            .Cast<string>()
                            .First(x => Regex.IsMatch(x, String.Format("{0}.*", item.StaticItem.Clipname)))
                            .Split('#')[1];

            byte[] packet = Encoding.ASCII.GetBytes(string.Format("spyder\0\0\0\0RSC {0} 1", item.ControlValue));

            client.Send(packet, packet.Length);
            client.Close();
        }

        public static string ConvertToHex(string asciiString)
        {
            string hex = "";
            foreach (char c in asciiString)
            {
                int tmp = c;
                hex += String.Format("{0:x2}", (uint)System.Convert.ToUInt32(tmp.ToString()));
            }
            return hex;
        } 


        
    }
}
