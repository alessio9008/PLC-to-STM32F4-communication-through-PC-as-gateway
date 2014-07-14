using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace CodeProjectSerialComms
{
    class PCLCommunication
    {
        private bool initSuccess = false;


        private libnodave.daveOSserialType fds;
        private libnodave.daveInterface di;
        private libnodave.daveConnection dc;
        private PLCState plc;
        private Param param;
        private Queue<Orientamento> microToPLC;


        public PCLCommunication(PLCState pl, Queue<Orientamento> list)
        {
            this.param = Param.getInstance();
            this.plc = pl;
            this.microToPLC = list;
        }

        public void start()
        {
            Thread thread = new Thread(new ThreadStart(send));
            thread.Start();
        }

        private void send()
        {
            if (connetti() == 0)
            {
                while (true)
                {
                    plc.setTextln("In attesa...");
                    Orientamento currentOrientamento = microToPLC.Waitremove();
                    plc.setTextln("Invio dati in corso....");
                    letturascrittuta(currentOrientamento);
                    plc.setTextln("Dati inviati");
                }
                //dc.disconnectPLC();
            }
            //disconnetti();
        }

        private int connetti()
        {
            int i, a = 0, j, res, b = 0, c = 0;
            float d = 0;
            byte[] buf1 = new byte[libnodave.davePartnerListSize];


            fds.rfd = libnodave.openS7online(param.s7online);
            fds.wfd = fds.rfd;
            if (fds.rfd >= 0)
            {
                di = new libnodave.daveInterface(fds, param.IF, param.localMPI, param.useProto, param.speed);
                di.setTimeout(param.timeOut);
                for (i = 0; i < 3; i++)
                {
                    if (0 == di.initAdapter())
                    {
                        initSuccess = true;
                        a = di.listReachablePartners(buf1);
                        plc.setTextln("daveListReachablePartners List length: " + a);
                        if (a > 0)
                        {
                            for (j = 0; j < a; j++)
                            {
                                if (buf1[j] == libnodave.daveMPIReachable)
                                    plc.setTextln("Device at address: " + j);
                            }
                        }
                        break;
                    }
                    else di.disconnectAdapter();
                }
                if (!initSuccess)
                {
                    plc.setTextln("Couldn't connect to Adapter!.\n Please try again. You may also try the option -2 for some adapters.");
                    return -3;
                }

                dc = new libnodave.daveConnection(di, param.plcMPI, 0, 0);

                return dc.connectPLC();

            }
            else
            {
                plc.setTextln("Couldn't open s7 online ");
                return -1;
            }
        }

        private void disconnetti()
        {
            di.disconnectAdapter();
            libnodave.closeS7online(fds.rfd);
            GC.Collect();
            GC.WaitForPendingFinalizers();
            plc.setTextln("Here we are");
        }

        private void letturascrittuta(Orientamento currentOrientamento)
        {
            int res = -1;
            byte[] buff;

            plc.setTextln("*************Scrittura memoria M" + param.asseX + " Asse x:***************");
            buff = new byte[] { currentOrientamento.Xbyte };
            res = dc.writeBytes(libnodave.daveFlags, 0, param.asseX, param.numByteMemWrite, buff);
            plc.setTextln("function result:" + res + "=" + libnodave.daveStrerror(res));
            if (res == 0) plc.setTextln("Valore byte scritto: " + buff[0]);

            plc.setTextln("*************Scrittura memoria M" + param.asseY + " Asse x:***************");
            buff = new byte[] { currentOrientamento.Ybyte };
            res = dc.writeBytes(libnodave.daveFlags, 0, param.asseY, param.numByteMemWrite, buff);
            plc.setTextln("function result:" + res + "=" + libnodave.daveStrerror(res));
            if (res == 0) plc.setTextln("Valore byte scritto: " + buff[0]);

            plc.setTextln("*************Scrittura memoria M" + param.asseZ + " Asse x:***************");
            buff = new byte[] { currentOrientamento.Zbyte };
            res = dc.writeBytes(libnodave.daveFlags, 0, param.asseZ, param.numByteMemWrite, buff);
            plc.setTextln("function result:" + res + "=" + libnodave.daveStrerror(res));
            if (res == 0) plc.setTextln("Valore byte scritto: " + buff[0]);


            plc.setTextln("*************Scrittura memoria M" + param.datoPronto + " Dato pronto:***************");
            buff = new byte[] { param.dataready };
            res = dc.writeBytes(libnodave.daveFlags, 0, param.datoPronto, param.numByteMemWrite, buff);
            plc.setTextln("function result:" + res + "=" + libnodave.daveStrerror(res));
            if (res == 0) plc.setTextln("Valore byte scritto: " + buff[0]);

            Thread.Sleep(10);

            plc.setTextln("*************Output " + param.addressInputRead + " :***************");
            buff = new byte[param.numByteInputRead];
            res = dc.readBytes(libnodave.daveOutputs, 0, param.addressInputRead, param.numByteInputRead, buff);
            plc.setTextln("function result:" + res + "=" + libnodave.daveStrerror(res));
            if (res == 0)
            {
                plc.setTextln("Valore byte: " + buff[0]);
                calculateAngle((Param.State)buff[0]);
            }

        }

        private void calculateAngle(Param.State state)
        {
            State st = new State();
            switch (state)
            {
                case Param.State.ORIZZONTALE:
                    {
                        st.StandardPosition = param.StandardPositionOrizzontale;
                        st.Angle = 0;
                        plc.addLast(st);
                        plc.setTextln("ORIZZONTALE");
                        break;
                    }
                case Param.State.ORIZZONTALECAPOVOLTO:
                    {
                        st.StandardPosition = param.StandardPositionOrizzontaleCapovolto;
                        st.Angle = 0;
                        plc.addLast(st);
                        plc.setTextln("ORIZZONTALECAPOVOLTO");
                        break;
                    }
                case Param.State.VERTICALEORIZZONTALE90:
                    {
                        st.StandardPosition = param.StandardPositionVerticaleOrizzontale90;
                        st.Angle = 0;
                        plc.addLast(st);
                        plc.setTextln("VERTICALEORIZZONTALE90");
                        break;
                    }
                case Param.State.VERTICALEORIZZONTALE270:
                    {
                        st.StandardPosition = param.StandardPositionVerticaleOrizzontale270;
                        st.Angle = 0;
                        plc.addLast(st);
                        plc.setTextln("VERTICALEORIZZONTALE270");
                        break;
                    }
                case Param.State.VERTICALEALTO:
                    {
                        st.Angle = param.AngleVerticaleAlto;
                        plc.addLast(st);
                        plc.setTextln("VERTICALEALTO");
                        break;
                    }
                case Param.State.VERTICALEBASSO:
                    {
                        st.Angle = param.AngleVerticaleBasso;
                        plc.addLast(st);
                        plc.setTextln("VERTICALEBASSO");
                        break;
                    }

            }
        }
    }
}
