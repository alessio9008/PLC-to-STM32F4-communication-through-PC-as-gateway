using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace CodeProjectSerialComms
{
    [DataContract]
    [Serializable]
    public class Param
    {
        //Communicazione PLC memoria M
        [DataMember]
        public int datoPronto = 20;
        [DataMember]
        public int asseX = 25;
        [DataMember]
        public int asseY = 27;
        [DataMember]
        public int asseZ = 29;
        [DataMember]
        public string s7online = "/S7ONLINE";
        [DataMember]
        public int useProto = libnodave.daveProtoS7online;
        [DataMember]
        public int speed = libnodave.daveSpeed187k;
        [DataMember]
        public int timeOut = 5000000;
        [DataMember]
        public int localMPI = 0;
        [DataMember]
        public string IF = "IF1";
        [DataMember]
        public int plcMPI = 2;
        [DataMember]
        public int numByteMemWrite = 1;
        [DataMember]
        public int numByteInputRead = 1;
        [DataMember]
        public int addressInputRead = 124;
        [DataMember]
        public byte dataready = 1;

        //Comandi immagine:
        [DataMember]
        public float minAngleRotate = 2.5f;
        [DataMember]
        public int frequencyRefreshImage = 1;
        [DataMember]
        public int StandardPositionOrizzontale=0;
        [DataMember]
        public int StandardPositionOrizzontaleCapovolto = 180;
        [DataMember]
        public int StandardPositionVerticaleOrizzontale90 = 90;
        [DataMember]
        public int StandardPositionVerticaleOrizzontale270 = 270;
        [DataMember]
        public float AngleVerticaleAlto=30;
        [DataMember]
        public float AngleVerticaleBasso=-30;


        [DataMember]
        public int pollingPeriod = 500;
        //Configurazione Seriale
        [DataMember]
        public int[] BoundRate = { 300, 600, 1200, 2400, 9600, 14400, 19200, 38400, 57600, 115200 };
        [DataMember]
        public int[] DataBits = { 7, 8 };
        [DataMember]
        public string[] StopBits = { "One", "OnePointFive", "Two" };
        [DataMember]
        public string[] Parity = { "None", "Even", "Mark", "Odd", "Space" };
        [DataMember]
        public string[] HandShaking = { "None", "XOnXOff", "RequestToSend", "RequestToSendXOnXOff" };

        
        
        //Comandi Micro
        public enum Commands : byte { START = 0x01, STOP = 0X02, AZIONE1 = 0x04, AZIONE2 = 0x08, AZIONE3 = 0x10, AZIONE4 = 0x20 }

        //Stati ritornati dal plc
        public enum State : byte { ORIZZONTALE = 1, ORIZZONTALECAPOVOLTO = 2, VERTICALEORIZZONTALE90 = 4, VERTICALEORIZZONTALE270 = 8, VERTICALEALTO = 16, VERTICALEBASSO = 32 }




        //Serialize
        private static Param current = null;
        private static object myLock = new object();

        public static Param getInstance()
        {
            if (current == null)
            {
                lock (myLock)
                {
                    if (current == null)
                    {
                        current = new Param();
                    }
                }
            }
            return current;
        }

        public static void XmlSerializer()
        {
            XmlSerializer xsSubmit = new XmlSerializer(typeof(Param));
            FileStream xmlFileWrite = new FileStream("Param.xml", FileMode.OpenOrCreate);
            StreamWriter writer = new StreamWriter(xmlFileWrite);
            System.Xml.XmlWriter xml = XmlWriter.Create(writer);
            if (current != null) xsSubmit.Serialize(xml, current);
            writer.Close();
            xmlFileWrite.Close();
        }

        public static void XmlDeserialize()
        {
            XmlSerializer xsSubmit = new XmlSerializer(typeof(Param));
            FileStream xmlFileRead = new FileStream("Param.xml", FileMode.OpenOrCreate);
            StreamReader reader = new StreamReader(xmlFileRead);
            current = (Param)xsSubmit.Deserialize(reader);
            reader.Close();
            xmlFileRead.Close();
        }

        public static void JsonSerializer()
        {
            DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Param));
            FileStream jsonFileWrite = new FileStream("Param.js", FileMode.OpenOrCreate);
            if (current != null) ser.WriteObject(jsonFileWrite, current);
            jsonFileWrite.Close();
        }
        /// <summary>
        /// JSON Deserialization
        /// </summary>
        public static void JsonDeserialize()
        {
            DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Param));
            FileStream jsonFileRead = new FileStream("Param.js", FileMode.OpenOrCreate);
            current = (Param)ser.ReadObject(jsonFileRead);
            jsonFileRead.Close();
        }
    }
}
