using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Caspar_Pilot
{
    class CGDataListWrapper : Svt.Caspar.ICGDataContainer
    {
        private List<Svt.Caspar.CGDataPair> Data { get; set; }
        public CGDataListWrapper(List<Svt.Caspar.CGDataPair> list)
        {
            Data = list;
        }

        public string ToAMCPEscapedXml()
        {
            return Svt.Caspar.CGDataPair.ToEscapedXml(Data);
        }

        public string ToXml()
        {
            return Svt.Caspar.CGDataPair.ToXml(Data);
        }
    }
}
