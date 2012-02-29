using System;
using System.Collections.Generic;
using System.Text;

namespace Caspar_Pilot.Hosts
{
	public class ChannelInformation
	{
		public ChannelInformation()
		{
			Label = Hostname = string.Empty;
			Port = 5250;
			Channel = 1;
			Online = false;
			ArgbValue = System.Drawing.Color.White.ToArgb();
		}

		public ChannelInformation(string hostname, ushort port, ushort channel, string label, int argb)
		{
			Hostname = hostname;
			Port = port;
			Channel = channel;
			Label = label;
			Online = false;
			ArgbValue = argb;
		}

		public string Label { get; set; }
		public string Hostname { get; set; }
		public ushort Port { get; set; }
		public ushort Channel { get; set; }
		public bool Online { get; set; }
		public int ArgbValue { get; set; }

		public override string ToString()
		{
			return Label + ": (" + Hostname + ":" + Port + ", channel " + Channel + ")";
		}
	}

	public class ChannelInformationList : List<ChannelInformation>
	{
	}
}
