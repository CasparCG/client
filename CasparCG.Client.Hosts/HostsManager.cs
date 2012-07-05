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
        public event EventHandler<EventArgs> ValidConnectionsChanged;

        class HostData
        {
            public Dictionary<string, DeviceHolder> Devices = new Dictionary<string,DeviceHolder>();
            public Dictionary<string, ChannelInformation> ChannelInfos = new Dictionary<string,ChannelInformation>();
        }

        HostData data_ = new HostData();

		public void SetChannels(ChannelInformationList channels)
		{
			if (channels != null)
			{
                HostData newData = new HostData();
				foreach (ChannelInformation channelInfo in channels)
				{
					DeviceHolder device = null;

                    if (newData.Devices.ContainsKey(channelInfo.Hostname))
                        device = newData.Devices[channelInfo.Hostname];
                    else
					{
						//Get device from old collection or create new
						if (data_.Devices.ContainsKey(channelInfo.Hostname))
						{
                            device = data_.Devices[channelInfo.Hostname];
                            data_.Devices.Remove(channelInfo.Hostname);

							device.Channels.Clear();
						}
						else
							device = CreateDevice(channelInfo.Hostname, channelInfo.Port);

						//add device to new collection
						if (device != null)
						{
							newData.Devices.Add(channelInfo.Hostname, device);
						}
					}

                    if (device != null)
                    {
                        device.Channels.Add(channelInfo);
                        channelInfo.Online = (channelInfo.Channel <= device.CasparDevice.Channels.Count) ? device.HasValidConnection : false;
                    }

                    try
                    {
                        newData.ChannelInfos.Add(channelInfo.Label, channelInfo);
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Det verkar finnas flera Caspar outputs med samma namn. Hoppar över en...", "Varning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
				}

				if (connected)
				{
					//disconnect from unused devices
					foreach (DeviceHolder device in data_.Devices.Values)
					{
                        device.UpdatedMediafiles -= device_UpdatedMediafiles;
                        device.UpdatedTemplates -= device_UpdatedTemplates;
                        device.VaildConnectionChanged -= device_VaildConnectionChanged;
						device.CasparDevice.Disconnect();
					}

					//connect to new devices
					foreach (DeviceHolder device in newData.Devices.Values)
					{
						if (!device.CasparDevice.IsConnected)
							device.CasparDevice.Connect();
					}
				}

				//set new collection as current
                data_ = newData;
                SetValidConnections();
			}
		}

		bool connected = false;
		public void Connect()
		{
			connected = true;
			foreach (DeviceHolder device in data_.Devices.Values)
				device.CasparDevice.Connect();
		}

		public void Disconnect()
		{
			connected = false;
            foreach (DeviceHolder device in data_.Devices.Values)
				device.CasparDevice.Disconnect();
		}

		public IEnumerable<ChannelInformation> ChannelInfos
		{
			get
			{
				return data_.ChannelInfos.Values;
			}
		}

		public ChannelInformation GetChannelInfo(string label)
		{
            return (data_.ChannelInfos.ContainsKey(label)) ? data_.ChannelInfos[label] : null;
		}

		public Channel GetChannel(string label)
		{
            if (data_.ChannelInfos.ContainsKey(label))
			{
                ChannelInformation info = data_.ChannelInfos[label];
                if (data_.Devices.ContainsKey(info.Hostname))
				{
                    DeviceHolder device = data_.Devices[info.Hostname];
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
                return data_.Devices.Values;
			}
		}
		public DeviceHolder GetDevice(string hostname)
		{
            return (data_.Devices.ContainsKey(hostname)) ? data_.Devices[hostname] : null;
		}

		private DeviceHolder CreateDevice(string hostname, int port)
		{
			DeviceHolder device = new DeviceHolder(hostname, port);
            device.UpdatedMediafiles += device_UpdatedMediafiles;
            device.UpdatedTemplates += device_UpdatedTemplates;
            device.VaildConnectionChanged += device_VaildConnectionChanged;
            return device;
		}

        ValidHostsConnections validConnections_ = ValidHostsConnections.None;
        public ValidHostsConnections ValidConnections
        {
            get { return validConnections_; }
            set
            {
                if (validConnections_ != value)
                {
                    validConnections_ = value;

                    try
                    {
                        if (ValidConnectionsChanged != null)
                            ValidConnectionsChanged(this, EventArgs.Empty);
                    }
                    catch { }
                }
            }
        }

        void device_VaildConnectionChanged(object sender, EventArgs e)
        {
            SetValidConnections();
        }

        void SetValidConnections()
        {
            bool bAllConnected = true, bAllDisconnected = true;
            foreach (DeviceHolder device in data_.Devices.Values)
            {
                if (device.HasValidConnection)
                    bAllDisconnected = false;
                else
                    bAllConnected = false;
            }

            if (bAllConnected)
                ValidConnections = ValidHostsConnections.All;
            else if (bAllDisconnected)
                ValidConnections = ValidHostsConnections.None;
            else
                ValidConnections = ValidHostsConnections.Some;
        }

        void device_UpdatedMediafiles(object sender, EventArgs e)
        {
			if (UpdatedMediafiles != null)
				UpdatedMediafiles(this, new HostEventArgs((DeviceHolder)sender));
        }

        void device_UpdatedTemplates(object sender, EventArgs e)
        {
            if (UpdatedTemplates != null)
                UpdatedTemplates(this, new HostEventArgs((DeviceHolder)sender));
        }
	}

	public class HostEventArgs : EventArgs
	{
		internal HostEventArgs(DeviceHolder device)
		{
			Device = device;
		}
		public DeviceHolder Device { get; private set; }
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
        public event EventHandler<EventArgs> UpdatedMediafiles;
        public event EventHandler<EventArgs> UpdatedTemplates;

        public DeviceHolder(string hostname, int port)
		{
            Channels = new List<ChannelInformation>();
            Files = new List<MediaInfo>();
            Templates = TemplatesCollection.Empty;

            CasparDevice = new CasparDevice();
            CasparDevice.Settings.Hostname = hostname;
            CasparDevice.Settings.Port = port;
            CasparDevice.Settings.AutoConnect = true;

            CasparDevice.UpdatedChannels += caspar_UpdatedChannels;
            CasparDevice.ConnectionStatusChanged += casparDevice_ConnectionStatusChanged;

            CasparDevice.UpdatedTemplates += caspar_UpdatedTemplates;
            CasparDevice.UpdatedMediafiles += caspar_UpdatedMediafiles;
		}

		internal CasparDevice CasparDevice { get; private set; }
		public string Hostname { get { return CasparDevice.Settings.Hostname; } }

        public List<Svt.Caspar.MediaInfo> Files { get; private set; }
        public TemplatesCollection Templates { get; private set; }
		
        public List<ChannelInformation> Channels { get; private set; }

        #region caspar events
        void casparDevice_ConnectionStatusChanged(object sender, Svt.Network.ConnectionEventArgs e)
        {
            CasparDevice casparDevice = (CasparDevice)sender;
            if (e.Connected)
            {
                if(Files.Count == 0)
                    casparDevice.RefreshMediafiles();

                if (Templates.All.Count == 0)
                    casparDevice.RefreshTemplates();
            }
            else
            {
                foreach (ChannelInformation channel in Channels)
                    channel.Online = false;

                HasValidConnection = false;
            }
        }
        
        void caspar_UpdatedChannels(object sender, EventArgs e)
        {
            foreach (ChannelInformation channel in Channels)
            {
                if (channel.Channel <= CasparDevice.Channels.Count)
                    channel.Online = true;
                else
                    channel.Online = false;
            }

            HasValidConnection = true;
        }

        private void caspar_UpdatedMediafiles(object sender, EventArgs e)
        {
            Files = CasparDevice.Mediafiles;

            try
            {
                if (UpdatedMediafiles != null)
                    UpdatedMediafiles(this, EventArgs.Empty);
            }
            catch { }
        }

        private void caspar_UpdatedTemplates(object sender, EventArgs e)
        {
            Templates = CasparDevice.Templates;

            try
            {
                if (UpdatedTemplates != null)
                    UpdatedTemplates(this, EventArgs.Empty);
            }
            catch { }
        }
        #endregion
        
        bool hasVaildConnection_ = false;
		public bool HasValidConnection 
		{ 
			get { return hasVaildConnection_; }
			set
			{
                if (hasVaildConnection_ != value)
                {
                    hasVaildConnection_ = value;
                    try
                    {
                        if (VaildConnectionChanged != null)
                            VaildConnectionChanged(this, EventArgs.Empty);
                    }
                    catch { }
                }
			}
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
