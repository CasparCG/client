using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Caspar_Pilot
{
	public partial class SettingsDialog : Form
	{
//		static string SameChannelWarning = "OBS! Det är inte rekomenderat att köra mallgrafik och förrenderad grafik på samma kanal i samma sändning om man inte verkligen vet vad man ger sig in på.";
		
		public SettingsDialog()
		{
			InitializeComponent();
		}

		private void SettingsDialog_Load(object sender, EventArgs e)
		{
		}

		#region autofields
		public string DefaultFieldPrefix
		{
			get { return tfieldPrefix.Text; }
			set { tfieldPrefix.Text = value; }
		}
		public int DefaultFieldStartIndex
		{
			get { return Convert.ToInt32(nFieldStartIndex.Value); }
			set { nFieldStartIndex.Value = value; }
		}
		#endregion

		#region Offline-filer
		public string LocalTemplateFolder
		{
			get { return tTemplateFolder_.Text; }
			set { tTemplateFolder_.Text = value; }
		}
		private void btnBrowse__Click(object sender, EventArgs e)
		{
			try
			{
				FolderBrowserDialog fbd = new FolderBrowserDialog();
				fbd.ShowNewFolderButton = true;
				fbd.RootFolder = Environment.SpecialFolder.MyComputer;
				if (!string.IsNullOrEmpty(tTemplateFolder_.Text))
				{
					string fullPath = System.IO.Path.GetFullPath(tTemplateFolder_.Text);
					if (System.IO.Directory.Exists(fullPath))
						fbd.SelectedPath = fullPath;
				}
				if (fbd.ShowDialog() == DialogResult.OK)
				{
					tTemplateFolder_.Text = fbd.SelectedPath;
					if (tTemplateFolder_.Text.StartsWith(Environment.CurrentDirectory) && tTemplateFolder_.Text.Length > Environment.CurrentDirectory.Length)
						tTemplateFolder_.Text = tTemplateFolder_.Text.Substring(Environment.CurrentDirectory.Length+1);
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Unable to browse the offline folder");
			}
		}
		#endregion

		#region Färger
		public Color TemplateColor
        {
            get 
            {
                Color temp = Color.FromName(lvColor_.Items[0].SubItems[1].Text);

                if (temp.A == 0)
                {
                    temp = Color.LightBlue;
                    MessageBox.Show("Invalid template color");
                }

                return temp;
            }
            set
            {
                lvColor_.Items[0].SubItems[1].Text = ((Color)value).Name;
            }
        }

        public Color GraphicColor
        {
            get
            {
                Color temp = Color.FromName(lvColor_.Items[1].SubItems[1].Text);

                if (temp.A == 0)
                {
                    temp = Color.LightGreen;
                    MessageBox.Show("Invalid graphic color");
                }

                return temp;
            }
            set
            {
               lvColor_.Items[1].SubItems[1].Text = ((Color)value).Name;
            }
        }

        public Color MultistepColor
        {
            get
            {
                Color temp = Color.FromName(lvColor_.Items[2].SubItems[1].Text);

                if (temp.A == 0)
                {
                    temp = Color.CornflowerBlue;
                    MessageBox.Show("Invalid multistep color");
                }

                return temp;
            }
            set
            {
               lvColor_.Items[2].SubItems[1].Text = ((Color)value).Name;
            }
        }

        private void lvColor__SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvColor_.SelectedItems.Count > 0)
            {
                txtColor_.Text = lvColor_.SelectedItems[0].Text;
                cbColor_.Text = lvColor_.SelectedItems[0].SubItems[1].Text;
            }
        }

        private void cbColor__TextChanged(object sender, EventArgs e)
        {
            if(lvColor_.SelectedItems.Count > 0)
                lvColor_.SelectedItems[0].SubItems[1].Text = cbColor_.Text;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            TemplateColor = Color.LightBlue;
            GraphicColor = Color.LightGreen;
            MultistepColor = Color.CornflowerBlue;
		}
		#endregion

		#region Hotkeys
		public Dictionary<string, Keys> HotKeys
        {
            get
            {
                Dictionary<string, Keys> temp = new Dictionary<string, Keys>();
                try
                {                    
                    temp["Load"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[0].SubItems[1].Text);
                }
                catch (Exception)
                {
                    temp["Load"] = Keys.F1;
                    MessageBox.Show("Invalid hotkey for \"Load\".");
                }
                try
                {
                    temp["Play"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[1].SubItems[1].Text);
                }
                catch (Exception)
                {
                    temp["Play"] = Keys.F2;
                    MessageBox.Show("Invalid hotkey for \"Play\".");
                }
                try
                {
                    temp["Step"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[2].SubItems[1].Text);
                }
                catch (Exception)
                {
					temp["Step"] = Keys.F3;
					MessageBox.Show("Invalid hotkey for \"Step\".");
                }

                try
                {
                    temp["Update"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[3].SubItems[1].Text);
                }
                catch (Exception)
                {
                    temp["Update"] = Keys.F5;
                    MessageBox.Show("Invalid hotkey for \"Update\".");
                }

                try
                {
                    temp["Stop"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[4].SubItems[1].Text);
                }
                catch (Exception)
                {
                    temp["Stop"] = Keys.F4;
                    MessageBox.Show("Invalid hotkey for \"Stop\".");
                }
				try
				{
					temp["QuickPlay"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[5].SubItems[1].Text);
				}
				catch (Exception)
				{
					temp["QuickPlay"] = Keys.F12;
					MessageBox.Show("Invalid hotkey for \"QuickPlay\".");
				}
				try
				{
					temp["QuickStop"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[6].SubItems[1].Text);
				}
				catch (Exception)
				{
					temp["QuickStop"] = Keys.F9;
					MessageBox.Show("Invalid hotkey for \"QuickStop\".");
				}
				try
				{
					temp["QuickStep"] = (Keys)Enum.Parse(typeof(Keys), lvHotKey_.Items[7].SubItems[1].Text);
				}
				catch (Exception)
				{
					temp["QuickStep"] = Keys.F11;
					MessageBox.Show("Invalid hotkey for \"QuickStep\".");
				}     

                return temp;
            }

            set
            {
                lvHotKey_.Items[0].SubItems[1].Text = value["Load"].ToString();
                lvHotKey_.Items[1].SubItems[1].Text = value["Play"].ToString();
                lvHotKey_.Items[2].SubItems[1].Text = value["Step"].ToString();
                lvHotKey_.Items[3].SubItems[1].Text = value["Update"].ToString();
                lvHotKey_.Items[4].SubItems[1].Text = value["Stop"].ToString();
				lvHotKey_.Items[5].SubItems[1].Text = value["QuickPlay"].ToString();
				lvHotKey_.Items[6].SubItems[1].Text = value["QuickStop"].ToString();
				lvHotKey_.Items[7].SubItems[1].Text = value["QuickStep"].ToString();
			}
        }

        private void lvHotKey__SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvHotKey_.SelectedItems.Count > 0)
            {
                txtAction_.Text = lvHotKey_.FocusedItem.Text;
                txtHotKey_.Text = lvHotKey_.FocusedItem.SubItems[1].Text;
            }
        }

        private void txtHotKey__TextChanged(object sender, EventArgs e)
        {
            if (lvHotKey_.SelectedItems.Count > 0)
            {
                lvHotKey_.SelectedItems[0].SubItems[1].Text = txtHotKey_.Text;               
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            lvHotKey_.Items[0].SubItems[1].Text = "F1";
            lvHotKey_.Items[1].SubItems[1].Text = "F2";
            lvHotKey_.Items[2].SubItems[1].Text = "F3";
            lvHotKey_.Items[3].SubItems[1].Text = "F5";
            lvHotKey_.Items[4].SubItems[1].Text = "F4";
			lvHotKey_.Items[5].SubItems[1].Text = "F12";
			lvHotKey_.Items[6].SubItems[1].Text = "F9";
			lvHotKey_.Items[7].SubItems[1].Text = "F11";
		}
		#endregion

		private void lHosts__SelectedIndexChanged(object sender, EventArgs e)
		{
			btnDeleteHost_.Enabled = btnSaveHost_.Enabled = (lHosts_.SelectedIndex != ListBox.NoMatches);
			if (lHosts_.SelectedIndex != ListBox.NoMatches)
			{
				Hosts.ChannelInformation info = (Hosts.ChannelInformation)lHosts_.SelectedItem;
				tEditHostTitle_.Text = info.Label;
				tEditHostname_.Text = info.Hostname;
				nEditHostChannel_.Value = info.Channel;
				nEditHostPort_.Value = info.Port;
				panColorpick.BackColor = Color.FromArgb(info.ArgbValue);
			}
		}

		private void panColorpick_Click(object sender, EventArgs e)
		{
			ColorDialog cd = new ColorDialog();
			if (cd.ShowDialog() == DialogResult.OK)
			{
				panColorpick.BackColor = cd.Color;
			}
		}

		private void tEditHostTextbox__TextChanged(object sender, EventArgs e)
		{
			btnAddHost_.Enabled = (tEditHostname_.Text.Length > 0 && tEditHostTitle_.Text.Length > 0);
		}

		private void btnAddHost__Click(object sender, EventArgs e)
		{
            // TODO: Add check for duplicate hostname. This seems to be unique -> HostManager.cs, Ln 58.
			lHosts_.Items.Add(new Hosts.ChannelInformation(tEditHostname_.Text, Convert.ToUInt16(nEditHostPort_.Value), Convert.ToUInt16(nEditHostChannel_.Value), tEditHostTitle_.Text, panColorpick.BackColor.ToArgb()));
		}

		private void btnSaveHost__Click(object sender, EventArgs e)
		{
			if (lHosts_.SelectedIndex != ListBox.NoMatches)
			{
				lHosts_.Items[lHosts_.SelectedIndex] = new Hosts.ChannelInformation(tEditHostname_.Text, Convert.ToUInt16(nEditHostPort_.Value), Convert.ToUInt16(nEditHostChannel_.Value), tEditHostTitle_.Text, panColorpick.BackColor.ToArgb());
			}
		}

		private void btnDeleteHost__Click(object sender, EventArgs e)
		{
			if (lHosts_.SelectedIndex != ListBox.NoMatches)
				lHosts_.Items.RemoveAt(lHosts_.SelectedIndex);
		}

		public Hosts.ChannelInformationList Channels
		{
			get
			{
				Hosts.ChannelInformationList result = new Hosts.ChannelInformationList();
				foreach (object obj in lHosts_.Items)
				{
					result.Add((Hosts.ChannelInformation)obj);
				}
				return result;
			}

			set
			{
				lHosts_.SuspendLayout();
				try
				{
					lHosts_.Items.Clear();
					if(value != null)
					{
						foreach (Hosts.ChannelInformation info in value)
						{
							lHosts_.Items.Add(info);
						}
					}
				}
				finally
				{
					lHosts_.ResumeLayout();
				}
			}
		}

		public Color PreviewBackgroundColor { get { return pBGColor.BackColor; } set { pBGColor.BackColor = value; } }
		private void pBGColor_DoubleClick(object sender, EventArgs e)
		{
			ColorDialog cd = new ColorDialog();
			cd.Color = pBGColor.BackColor;

			if (cd.ShowDialog() == DialogResult.OK)
			{
				pBGColor.BackColor = cd.Color;
			}
		}
	}
}
