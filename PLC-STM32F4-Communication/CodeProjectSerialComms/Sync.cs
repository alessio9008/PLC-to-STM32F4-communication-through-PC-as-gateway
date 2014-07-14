using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace CodeProjectSerialComms
{
    public class Sync
    {
        private bool sync = false;

        public void attendi(){
            lock (this)
            {
                while (sync == false) Monitor.Wait(this);
                sync = false;
            }
        }

        public void sveglia()
        {
            lock (this)
            {
                sync = true;
                Monitor.PulseAll(this);
            }
        }
    }
}
