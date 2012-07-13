using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Svt.Caspar;

namespace Caspar_Pilot
{
	class RundownItem : ICloneable
	{
        public CasparItem StaticItem { get; set; }
        public CasparCGItem CGItem { get; set; }
        public bool IsCG { get; set; }
        public bool IsControl { get; set; }
        public string Name { get; set; }
        public bool MultiStep { get; set; }
        public bool IsStoredData { get; set; }
        public bool IsDataUpdate { get; set; }
        public bool IsStarted { get; set; }
        public string Channel { get; set; }
        public Color Color { get; set; }
        public Color ChannelColor { get; set; }
        public string Page { get; set; }
        public int ControlValue { get; set; }
        public bool Online { get; set; }

		public RundownItem()
		{
			StaticItem = new Svt.Caspar.CasparItem(string.Empty);
            StaticItem.Transition.Duration = 12;
            StaticItem.VideoLayer = -1;
			CGItem = new Svt.Caspar.CasparCGItem();
            CGItem.VideoLayer = -1;
			IsCG = true;
            IsControl = false;
			MultiStep = false;
			IsStoredData = false;
            IsDataUpdate = false;
			IsStarted = false; 
            Color = Color.FromName(Properties.Settings.Default.TemplateColor);
			Name = Channel = string.Empty;
            Page = string.Empty;

            Online = false;
		}

		

		#region ICloneable Members

		public object Clone()
		{
			RundownItem item = new RundownItem();

			if (StaticItem != null)
			{
				item.StaticItem = new Svt.Caspar.CasparItem(StaticItem.Clipname);
				item.StaticItem.Loop = StaticItem.Loop;
				item.StaticItem.Transition.Type = StaticItem.Transition.Type;
				item.StaticItem.Transition.Duration = StaticItem.Transition.Duration;
                item.StaticItem.VideoLayer = StaticItem.VideoLayer;
			}

			if (CGItem != null)
			{
				item.CGItem = new Svt.Caspar.CasparCGItem();
				item.CGItem.AutoPlay = CGItem.AutoPlay;
				item.CGItem.Layer = CGItem.Layer;
                item.CGItem.VideoLayer = CGItem.VideoLayer;
				item.CGItem.TemplateName = CGItem.TemplateName;
				item.CGItem.TemplateProfile = CGItem.TemplateProfile;

				foreach (Svt.Caspar.CGDataPair cgdata in CGItem.Data)
					item.CGItem.Data.Add(new Svt.Caspar.CGDataPair(cgdata.Name, cgdata.Value));
			}

			item.IsCG = IsCG;
            item.IsControl = IsControl;
			item.Name = Name;
			item.MultiStep = MultiStep;
			item.IsStoredData = IsStoredData;
            item.IsDataUpdate = IsDataUpdate;
			item.Channel = Channel;
			item.Color = Color;
            item.Page = Page;
            item.ControlValue = ControlValue;

            item.Online = Online;
            item.ChannelColor = ChannelColor;

			return item;
		}

		#endregion
	}
}
