using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows.Forms;

namespace CodeProjectSerialComms
{
    static class ProgramMain
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Param.getInstance();
            //Param.XmlSerializer();
            //Param.JsonSerializer();
            Param.XmlDeserialize();
            //Param.JsonDeserialize();
            Sync sync = new Sync();
            Queue<Orientamento> microToPLC = new Queue<Orientamento>();
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            PLCState plcState = new PLCState();
            Application.Run(new SerialCommunicationForm(sync, plcState,microToPLC));
         
        }
    }
}
