using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Physical
{
    internal class StaminaControl
    {
        #region 싱글톤
        public static StaminaControl Singleton { get; } = null;
        static StaminaControl() { Singleton = new StaminaControl(); }
        private StaminaControl() { }
        #endregion

    }
}
