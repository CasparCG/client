using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using Svt.Caspar;

namespace Caspar_Pilot.Hosts
{
	public class HostsManager
	{
		public event EventHandler<HostEventArgs> UpdatedMediafiles;
		public event EventHandler<HostEventArgs> UpdatedTemplates;

		Dictionary<string, DeviceHolder> devices_;
		Dictionary<string, ChannelInformation> channelInfos_;

		public void SetChannels(ChannelInformationList channels)
		{
			if (channels != null)
			{
				Dictionary<string, DeviceHolder> newDevices = new Dictionary<string, DeviceHolder>();
				Dictionary<string, ChannelInformation> newChannelInfos = new Dictionary<string, ChannelInformation>();
				foreach (ChannelInformation channelInfo in channels)
				{
					if (!newDevices.ContainsKey(channelInfo.Hostname))
					{
						DeviceHolder device = null;

						//Get device from old collection or create new
						if (devices_ != null && devices_.ContainsKey(channelInfo.Hostname))
						{
							device = devices_[channelInfo.Hostname];
							devices_.Remove(channelInfo.Hostname);

							device.Channels.Clear();
						}
						else
							device = CreateDevice(channelInfo.Hostname, channelInfo.Port);

						//add device to new collection
						if (device != null)
						{
							device.Channels.Add(channelInfo);
							channelInfo.Online = device.HasValidConnection;
							newDevices.Add(channelInfo.Hostname, device);
						}

					}
					else
					{
						newDevices[channelInfo.Hostname].Channels.Add(channelInfo);
						channelInfo.Online = newDevices[channelInfo.Hostname].HasValidConnection;
					}

                    try
                    {
                        newChannelInfos.Add(channelInfo.Label, channelInfo);
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Det verkar finnas flera Caspar outputs med samma namn. Hoppar över en...", "Varning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
				}

				if (connected)
				{
					//disconnect from unused devices
					if (devices_ != null)
					{
						foreach (DeviceHolder device in devices_.Values)
						{
							device.CasparDevice.Disconnect();
						}
					}

					//connect to new devices
					foreach (DeviceHolder device in newDevices.Values)
					{
						if (!device.CasparDevice.IsConnected)
							device.CasparDevice.Connect();
					}
				}

				//set new collection as current
				System.Threading.Interlocked.Exchange<Dictionary<string, DeviceHolder>>(ref devices_, newDevices);
				System.Threading.Interlocked.Exchange<Dictionary<string, ChannelInformation>>(ref channelInfos_, newChannelInfos);

				UpdateValidConnections();
			}
		}

		bool connected = false;
		public void Connect()
		{
			connected = true;
			if (devices_ != null)
			{
				foreach (DeviceHolder device in devices_.Values)
					device.CasparDevice.Connect();
			}
		}

		public void Disconnect()
		{
			connected = false;
			if (devices_ != null)
			{
				foreach (DeviceHolder device in devices_.Values)
					device.CasparDevice.Disconnect();
			}
		}

		public IEnumerable<ChannelInformation> ChannelInfos
		{
			get
			{
				return (channelInfos_ != null) ? channelInfos_.Values : null;
			}
		}
		public ChannelInformation GetChannelInfo(string label)
		{
			return (channelInfos_ != null && channelInfos_.ContainsKey(label)) ? channelInfos_[label] : null;
		}
		public Channel GetChannel(string label)
		{
			if (channelInfos_ != null && channelInfos_.ContainsKey(label))
			{
				ChannelInformation info = channelInfos_[label];
				if (devices_ != null && devices_.ContainsKey(info.Hostname))
				{
					DeviceHolder device = devices_[info.Hostname];
					if ((device.CasparDevice.Channels.Count >= info.Channel) && (info.Channel > 0))
						return device.CasparDevice.Channels[info.Channel - 1];
				}
			}
			return null;
		}

		public IEnumerable<DeviceHolder> Devices
		{
			get
			{
				return (devices_ != null) ? devices_.Values : null;
			}
		}
		public DeviceHolder GetDevice(string hostname)
		{
			return (devices_ != null && devices_.ContainsKey(hostname)) ? devices_[hostname] : null;
		}

		private DeviceHolder CreateDevice(string hostname, int port)
		{
			CasparDevice casparDevice = new CasparDevice();
			casparDevice.Settings.Hostname = hostname;
			casparDevice.Settings.Port = port;
			casparDevice.Connected += new EventHandler<Svt.Network.NetworkEventArgs>(caspar_Connected);
			casparDevice.Disconnected += new EventHandler<Svt.Network.NetworkEventArgs>(caspar_Disconnected);
			casparDevice.FailedConnect += new EventHandler<Svt.Network.NetworkEventArgs>(caspar_FailedConnect);
			casparDevice.OnAsyncException += new EventHandler<Svt.Network.ExceptionEventArgs>(casparDevice_OnAsyncException);
			casparDevice.UpdatedChannels += new EventHandler<EventArgs>(caspar_UpdatedChannels);
			casparDevice.UpdatedTemplates += new EventHandler<EventArgs>(caspar_UpdatedTemplates);
			casparDevice.UpdatedMediafiles += new EventHandler<EventArgs>(caspar_UpdatedMediafiles);

			return new DeviceHolder(casparDevice);
		}

		ValidHostsConnections hasValidConnections = ValidHostsConnections.None;
		public ValidHostsConnections HasValidConnections
		{
			get { return hasValidConnections; }
		}
		void UpdateValidConnections()
		{
			if (devices_ != null)
			{
				bool bAllConnected = true, bAllDisconnected = true;
				foreach (DeviceHolder device in devices_.Values)
				{
					if (device.HasValidConnection)
						bAllDisconnected = false;
					else
						bAllConnected = false;
				}

				if (bAllConnected)
					hasValidConnections = ValidHostsConnections.All;
				else if (bAllDisconnected)
					hasValidConnections = ValidHostsConnections.None;
				else
					hasValidConnections = ValidHostsConnections.Some;
			}
			else
				hasValidConnections = ValidHostsConnections.None;
		}

		#region caspar events
		void caspar_Connected(object sender, Svt.Network.NetworkEventArgs e)
		{
			CasparDevice casparDevice = (CasparDevice)sender;
			casparDevice.RefreshMediafiles();
		}

		void caspar_FailedConnect(object sender, Svt.Network.NetworkEventArgs e)
		{
			CasparDevice casparDevice = (CasparDevice)sender;
			if (casparDevice != null && devices_.ContainsKey(casparDevice.Settings.Hostname))
			{
				devices_[casparDevice.Settings.Hostname].HasValidConnection = false;
				UpdateValidConnections();
			}
		}

		void casparDevice_OnAsyncException(object sender, Svt.Network.ExceptionEventArgs e)
		{
			CasparDevice casparDevice = (CasparDevice)sender;
			if (casparDevice != null && devices_.ContainsKey(casparDevice.Settings.Hostname))
			{
				devices_[casparDevice.Settings.Hostname].HasValidConnection = false;
				UpdateValidConnections();
			}
		}

		void caspar_Disconnected(object sender, Svt.Network.NetworkEventArgs e)
		{
			CasparDevice casparDevice = (CasparDevice)sender;
			if (casparDevice != null && devices_.ContainsKey(casparDevice.Settings.Hostname))
			{
				devices_[casparDevice.Settings.Hostname].HasValidConnection = false;
				UpdateValidConnections();
			}
		}

		void caspar_UpdatedChannels(object sender, EventArgs e)
		{
			CasparDevice casparDevice = (CasparDevice)sender;
			if (casparDevice != null && devices_.ContainsKey(casparDevice.Settings.Hostname))
			{
				devices_[casparDevice.Settings.Hostname].HasValidConnection = true;
				UpdateValidConnections();
			}
		}

		private void caspar_UpdatedMediafiles(object sender, EventArgs e)
		{
			try
			{
				CasparDevice casparDevice = (CasparDevice)sender;
				if (devices_ != null && devices_.ContainsKey(casparDevice.Settings.Hostname))
				{
					OnUpdatedMediafiles(devices_[casparDevice.Settings.Hostname]);
				}
			}
			catch { }
		}
		private void caspar_UpdatedTemplates(object sender, EventArgs e)
		{
			try
			{
				CasparDevice casparDevice = (CasparDevice)sender;
				if (devices_ != null && devices_.ContainsKey(casparDevice.Settings.Hostname))
				{
					OnUpdatedTemplates(devices_[casparDevice.Settings.Hostname]);
				}
			}
			catch { }
		}
		#endregion

		protected void OnUpdatedMediafiles(DeviceHolder device)
		{
			if (UpdatedMediafiles != null)
				UpdatedMediafiles(this, new HostEventArgs(device));
		}
		protected void OnUpdatedTemplates(DeviceHolder device)
		{
			if (UpdatedTemplates != null)
				UpdatedTemplates(this, new HostEventArgs(device));
		}
	}

	public class HostEventArgs : EventArgs
	{
		internal HostEventArgs(DeviceHolder device)
		{
			Device = device;
		}
		public DeviceHolder Device { get; set; }
	}

	public enum ValidHostsConnections
	{
		None,
		Some,
		All
	}

	public class DeviceHolder
	{
		public event EventHandler<EventArgs> VaildConnectionChanged;

		public DeviceHolder(CasparDevice device)
		{
			CasparDevice = device;
			Channels = new List<ChannelInformation>();
		}

		internal CasparDevice CasparDevice { get; private set; }
		public string Hostname { get { return CasparDevice.Settings.Hostname; } }
		public List<Svt.Caspar.MediaInfo> Files { get { return CasparDevice.Mediafiles; } }
		public TemplatesCollection Templates { get { return CasparDevice.Templates; } }
		public List<ChannelInformation> Channels { get; private set; }
		bool hasVaildConnection_ = false;
		public bool HasValidConnection 
		{ 
			get { return hasVaildConnection_; }
			set
			{
				hasVaildConnection_ = value;
				OnValidConnectionChanged();
			}
		}

		protected void OnValidConnectionChanged()
		{
			foreach (ChannelInformation info in Channels)
				info.Online = hasVaildConnection_;

			if (VaildConnectionChanged != null)
				VaildConnectionChanged(this, EventArgs.Empty);
		}

		public void RefreshTemplates()
		{
			CasparDevice.RefreshTemplates();
		}
		public void RefreshMediafiles()
		{
			CasparDevice.RefreshMediafiles();
		}
	}
}
