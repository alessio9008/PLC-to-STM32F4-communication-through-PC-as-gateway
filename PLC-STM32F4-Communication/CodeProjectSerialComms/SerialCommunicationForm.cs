using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO.Ports; //Classe che ci permette di leggere la porta seriale
using System.Windows.Forms;
using System.Threading;

//CodeProjectSerialComms program 
//23/04/2013   16:29

namespace CodeProjectSerialComms
{
    public partial class SerialCommunicationForm : Form
    {
        //Crea una comPort che ha le impostazioni di default, ovvero bit/rate 9600bps 
        //nessun bit di parità un bit di stop e nessun controllo di flusso.
        private SerialPort ComPort;
        private Param param;
        internal delegate void SerialDataReceivedEventHandlerDelegate(object sender, SerialDataReceivedEventArgs e);
        internal delegate void SerialPinChangedEventHandlerDelegate(object sender, SerialPinChangedEventArgs e);
        private SerialPinChangedEventHandler SerialPinChangedEventHandler1;
        delegate void SetTextCallback(string text);
        //Stringa che rappresenta i dati ricevuti tramite la seriale
        private string InputData = String.Empty;
        private PLCState plcState;
        private bool isStarted = false;
        private Sync syncPolling;
        private Queue<Orientamento> microToPLC;


        public SerialCommunicationForm(Sync syn, PLCState plcS, Queue<Orientamento> list)
        {
            this.microToPLC = list;
            this.syncPolling = new Sync();
            this.ComPort = new SerialPort();
            this.param = Param.getInstance();
            this.plcState = plcS;
            init();
        }

        private void init()
        {
            InitializeComponent();
            Size sizeScreen = Screen.PrimaryScreen.WorkingArea.Size;
            this.Bounds = new Rectangle(new Point(sizeScreen.Width - this.Size.Width, sizeScreen.Height - this.Size.Height), this.Size);
            ComPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(port_DataReceived_1);
            plcState.Show();
            PCLCommunication plc = new PCLCommunication(plcState,microToPLC);
            plc.start();
            Thread polling = new Thread(new ThreadStart(executePolling));
            polling.Start();
        }

        //Metodo richiamato quando clicckiamo su Ports     
        private void btnGetSerialPorts_Click(object sender, EventArgs e)
        {
            string[] ArrayComPortsNames = null;
            int index = 0;
            string ComPortName = null;

            //Ottiene le comPort sotto forma di matrice
            ArrayComPortsNames = SerialPort.GetPortNames();
            //Il loop va avanti fino a quando non abbiamo una porta COM a null, o l'indice arriva al limite
            //dell'array.
            //ArrayComPortsNames.GetUpperBound(0) ottiene la dimensione dell'array corrispondente alla prima
            //riga o colonna.
            try
            {
                while (!((ArrayComPortsNames[index] == ComPortName) || (index == ArrayComPortsNames.GetUpperBound(0))))
                {

                    //Aggiunge le com port alla ComboBox (Elenco)
                    cboPorts.Items.Add(ArrayComPortsNames[index]);

                    index++;
                }
                //Ordina l'array
                Array.Sort(ArrayComPortsNames);

                //se l'indice arriva al limite dell'array.
                if (index == ArrayComPortsNames.GetUpperBound(0))
                {
                    //Seleziona la prima porta com
                    ComPortName = ArrayComPortsNames[0];
                }
                //get first item print in text
                cboPorts.Text = ArrayComPortsNames[0].ToString();

                //Baud Rate
                for (int counter = 0; counter < param.BoundRate.Length; counter++) cboBaudRate.Items.Add(param.BoundRate[counter]);
                cboBaudRate.Items.ToString();
                //get first item print in text
                cboBaudRate.Text = cboBaudRate.Items[4].ToString();

                //Data Bits
                for (int counter = 0; counter < param.DataBits.Length; counter++) cboDataBits.Items.Add(param.DataBits[counter]);
                //get the first item print it in the text 
                cboDataBits.Text = cboDataBits.Items[1].ToString();

                //Stop Bits
                for (int counter = 0; counter < param.StopBits.Length; counter++) cboStopBits.Items.Add(param.StopBits[counter]);
                //get the first item print in the text
                cboStopBits.Text = cboStopBits.Items[0].ToString();


                //Parity 
                for (int counter = 0; counter < param.Parity.Length; counter++) cboParity.Items.Add(param.Parity[counter]);
                //get the first item print in the text
                cboParity.Text = cboParity.Items[0].ToString();

                //Handshake
                for (int counter = 0; counter < param.HandShaking.Length; counter++) cboHandShaking.Items.Add(param.HandShaking[counter]);
                //get the first item print it in the text 
                cboHandShaking.Text = cboHandShaking.Items[0].ToString();
            }
            catch (System.IndexOutOfRangeException)
            {
                MessageBox.Show("Non trovo porte COM disponibili", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }


        //Metodo che viene eseguito quando si riceve un dato
        private void port_DataReceived_1(object sender, SerialDataReceivedEventArgs e)
        {
            InputData = ComPort.ReadExisting();
            if (InputData != String.Empty)
            {
                //Esegue il delegato in modo asincrono e permette di stampare il dato nella RichTextBox
                this.BeginInvoke(new SetTextCallback(SetText), new object[] { InputData });
            }
        }

        //Viene invocato per aggiungere testo alla RichTextBox
        private void SetText(string text)
        {
            //Append del Testo 
            string[] subtext = text.Split(';');
            foreach (string tmp in subtext)
            {
                if (tmp != String.Empty && tmp != "")
                {
                    string[] split = tmp.Split(',');
                    Orientamento currentOrientamento = new Orientamento();
                    foreach (string test in split)
                    {
                        if (test != String.Empty && test != "")
                        {
                            string[] assi = test.Split(':');

                            byte asse = 10;
                            try
                            {
                                asse=Convert.ToByte(assi[0]);
                            }
                            catch (System.Exception) {
                                asse = 10;
                            }
                            switch (asse)
                            {
                                case 0: {
                                    try
                                    {
                                        currentOrientamento.X = Convert.ToSByte(assi[1], 2);
                                    }
                                    catch (System.Exception) {
                                        currentOrientamento.X = 0;
                                    }
                                    break; 
                                }
                                case 2: {
                                    try
                                    {
                                        currentOrientamento.Y = Convert.ToSByte(assi[1], 2);
                                    }
                                    catch (System.Exception) {
                                        currentOrientamento.Y = 0;
                                    }
                                    break; 
                                }
                                case 4: {
                                    try
                                    {
                                        currentOrientamento.Z = Convert.ToSByte(assi[1], 2);
                                    }
                                    catch (System.Exception) {
                                        currentOrientamento.Z = 0;
                                    }
                                    break; 
                                }
                            }
                            
                        }
                    }
                    if(!currentOrientamento.checkEqual(microToPLC.Tail))microToPLC.Waitinsert(currentOrientamento);
                    this.rtbIncoming.Text = currentOrientamento.ToString() + "\n" + this.rtbIncoming.Text;
                }
            }

        }


        private void btnPortState_Click(object sender, EventArgs e)
        {
            try
            {
                if (btnPortState.Text == "Closed")
                {
                    btnPortState.Text = "Open";
                    ComPort.PortName = Convert.ToString(cboPorts.Text);
                    ComPort.BaudRate = Convert.ToInt32(cboBaudRate.Text);
                    ComPort.DataBits = Convert.ToInt16(cboDataBits.Text);
                    ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cboStopBits.Text);
                    ComPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), cboHandShaking.Text);
                    ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), cboParity.Text);
                    try
                    {
                        ComPort.Open();
                    }
                    catch (System.IO.IOException)
                    {
                        MessageBox.Show("Errore nell'apertura della connessione", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else if (btnPortState.Text == "Open")
                {
                    btnPortState.Text = "Closed";
                    try
                    {
                        ComPort.Close();
                    }
                    catch (System.IO.IOException)
                    {
                        MessageBox.Show("Errore nella connessione", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }

                }
            }
            catch (System.ArgumentException)
            {
                MessageBox.Show("Connessione non aperta", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void executePolling()
        {
            while (true)
            {
                while (!isStarted) syncPolling.attendi();
                Thread.Sleep(param.pollingPeriod);
                byte[] command = { (byte)Param.Commands.START, 0x00 };
                ComPort.Write(command, 0, 2);
            }

        }

        //Usata quando viene premuto il tasto send
        private void btnCommand_Click(object sender, EventArgs e)
        {
            byte[] command = { (byte)System.Enum.Parse(typeof(Param.Commands), cboCommand.SelectedValue.ToString(), false), 0x00 };
            try
            {
                if (((Param.Commands)command[0]) == Param.Commands.START)
                {
                    isStarted = true;
                    syncPolling.sveglia();
                }
                else isStarted = false;
                ComPort.Write(command, 0, 2);
            }
            catch (System.InvalidOperationException)
            {
                MessageBox.Show("Hai dimenticato di Aprire la Connessione", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (System.IO.IOException)
            {
                MessageBox.Show("Errore comunicazione", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void brnClear_Click(object sender, EventArgs e)
        {
            rtbIncoming.Clear();
        }

        private void chPLC_CheckedChanged(object sender, EventArgs e)
        {
            if (chPLC.Checked)
            {
                plcState.Show();
            }
            else
            {
                plcState.Hide();
            }
        }

        private void SerialCommunicationForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Environment.Exit(0);
        }




    }
}
