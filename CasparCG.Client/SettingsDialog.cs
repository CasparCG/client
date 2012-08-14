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
            LocalTemplateFolder = Properties.Settings.Default.TemplateFolder;

            TemplateColor = Color.FromName(Properties.Settings.Default.TemplateColor);
            GraphicColor = Color.FromName(Properties.Settings.Default.GraphicColor);
            MultistepColor = Color.FromName(Properties.Settings.Default.MultistepColor);

            DefaultFieldPrefix = Properties.Settings.Default.DefaultFieldNamePrefix;
            DefaultFieldStartIndex = Properties.Settings.Default.DefaultFieldNameStartIndex;
            Channels = Properties.Settings.Default.Channels;

            PreviewBackgroundColor = Properties.Settings.Default.PreviewBackgroundColor;

            InitializeHotkeys();
		}

        private void SettingsDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == System.Windows.Forms.DialogResult.OK)
            {
                Properties.Settings.Default.TemplateFolder = LocalTemplateFolder;

                Properties.Settings.Default.TemplateColor = TemplateColor.Name;
                Properties.Settings.Default.GraphicColor = GraphicColor.Name;
                Properties.Settings.Default.MultistepColor = MultistepColor.Name;

                Properties.Settings.Default.DefaultFieldNamePrefix = DefaultFieldPrefix;
                Properties.Settings.Default.DefaultFieldNameStartIndex = DefaultFieldStartIndex;
                Properties.Settings.Default.Channels = Channels;

                Properties.Settings.Default.PreviewBackgroundColor = PreviewBackgroundColor;

                EnableHotkeys();

                Properties.Settings.Default.Save();
            }
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

        #region hotkeys
        public ToolStripMenuItem PlayoutMenuItems { get; set; }
        public ToolStripMenuItem CasparMenuItems { get; set; }

        Caspar_Pilot.Controls.HotkeyEditControl hotkeyEditCtrl;
        void InitializeHotkeys()
        {
            hotkeyEditCtrl = new Caspar_Pilot.Controls.HotkeyEditControl();
            hotkeyEditCtrl.Visible = false;
            lvHotKey_.Parent.Controls.Add(hotkeyEditCtrl);
            lvHotKey_.SubItemClicked += lvHotKey__SubItemClicked;

            TypeConverter converter = TypeDescriptor.GetConverter(typeof(Keys));

            if(PlayoutMenuItems != null && PlayoutMenuItems.HasDropDownItems)
            {
                foreach(ToolStripItem menuItem in PlayoutMenuItems.DropDownItems)
                {
                    if (menuItem.Tag != null)
                    {
                        ListViewItem lvItem = new ListViewItem(lvHotKey_.Groups[0]);
                        lvItem.Text = menuItem.Text;
                        lvItem.Name = menuItem.Text;
                        lvItem.ToolTipText = menuItem.Text;
                        lvItem.SubItems.Add(converter.ConvertToString(Properties.Settings.Default[menuItem.Tag.ToString()]));
                        lvHotKey_.Items.Add(lvItem);
                    }
                }
            }

            if(CasparMenuItems != null && CasparMenuItems.HasDropDownItems)
            {
                foreach (ToolStripItem menuItem in CasparMenuItems.DropDownItems)
                {
                    if (menuItem.Tag != null)
                    {
                        ListViewItem lvItem = new ListViewItem(lvHotKey_.Groups[1]);
                        lvItem.Text = menuItem.Text;
                        lvItem.Name = menuItem.Text;
                        lvItem.ToolTipText = menuItem.Text;
                        lvItem.SubItems.Add(converter.ConvertToString(Properties.Settings.Default[menuItem.Tag.ToString()]));
                        lvHotKey_.Items.Add(lvItem);
                    }
                }
            }
        }

        private void EnableHotkeys()
        {
            if (PlayoutMenuItems != null && PlayoutMenuItems.HasDropDownItems)
            {
                TypeConverter converter = TypeDescriptor.GetConverter(typeof(Keys));

                //iterate all submenuitems
                foreach (ToolStripItem item in PlayoutMenuItems.DropDownItems)
                {
                    ToolStripMenuItem menuItem = item as ToolStripMenuItem;
                    if (menuItem != null && menuItem.Tag != null)
                    {
                        //find listview item with same name
                        ListViewItem lvItem = lvHotKey_.Items[menuItem.Text];
                        if (lvItem != null && lvItem.SubItems.Count > 1)
                        {
                            //assign new shortcut to menuitem
                            Keys shortcut = (Keys)converter.ConvertFromString(lvItem.SubItems[1].Text);
                            menuItem.ShortcutKeys = shortcut;
                            Properties.Settings.Default[menuItem.Tag.ToString()] = shortcut;
                        }

                    }
                }
            }

            if (CasparMenuItems != null && CasparMenuItems.HasDropDownItems)
            {
                TypeConverter converter = TypeDescriptor.GetConverter(typeof(Keys));

                //iterate all submenuitems
                foreach (ToolStripItem item in CasparMenuItems.DropDownItems)
                {
                    ToolStripMenuItem menuItem = item as ToolStripMenuItem;
                    if (menuItem != null && menuItem.Tag != null)
                    {
                        //find listview item with same name
                        ListViewItem lvItem = lvHotKey_.Items[menuItem.Text];
                        if (lvItem != null && lvItem.SubItems.Count > 1)
                        {
                            //assign new shortcut to menuitem
                            Keys shortcut = (Keys)converter.ConvertFromString(lvItem.SubItems[1].Text);
                            menuItem.ShortcutKeys = shortcut;
                            Properties.Settings.Default[menuItem.Tag.ToString()] = shortcut;
                        }

                    }
                }
            }
        }

        void lvHotKey__SubItemClicked(object sender, SubItemEventArgs e)
        {
            if (e.Item != null)
            {
                lvHotKey_.StartEditing(hotkeyEditCtrl, e.Item, e.SubItem);
            }
        }
        #endregion

        #region hosts
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
        #endregion

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
