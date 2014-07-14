using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeProjectSerialComms
{
    public class Orientamento
    {
        private sbyte x = 0, y = 0, z = 0;

        private static Orientamento current = null;


        public Orientamento(sbyte x,sbyte y,sbyte z) { 
        
        }

        public Orientamento() { }


        public byte this[char asse, byte index]
        {
            get
            {
                if (index >= 0 && index < 8)
                {
                    switch (asse)
                    {
                        case 'x': { return (byte)((x >> index) & 0x01); }
                        case 'y': { return (byte)((y >> index) & 0x01); }
                        case 'z': { return (byte)((z >> index) & 0x01); }
                        case 'X': { return (byte)((x >> index) & 0x01); }
                        case 'Y': { return (byte)((y >> index) & 0x01); }
                        case 'Z': { return (byte)((z >> index) & 0x01); }
                        default: return 255;

                    }
                }
                return 255;
            }

            set {
                byte val = (byte)((value >= 1 || value < 0) ? 1 : 0);
                if (index >= 0 && index < 8)
                {
                    switch (asse)
                    {
                        case 'x':
                            {
                                x &= (sbyte)~(1 << index);
                                x |= (sbyte)(val << index);
                                break;
                            }
                        case 'y':
                            {
                                y &= (sbyte)~(1 << index);
                                y |= (sbyte)(val << index);
                                break;
                            }
                        case 'z':
                            {
                                z &= (sbyte)~(1 << index);
                                z |= (sbyte)(val << index);
                                break;
                            }
                        case 'X':
                            {
                                x &= (sbyte)~(1 << index);
                                x |= (sbyte)(val << index);
                                break;
                            }
                        case 'Y':
                            {
                                y &= (sbyte)~(1 << index);
                                y |= (sbyte)(val << index);
                                break;
                            }
                        case 'Z':
                            {
                                z &= (sbyte)~(1 << index);
                                z |= (sbyte)(val << index);
                                break;
                            }
                        default: break;

                    }
                }
            }
        }

        public byte Xbyte
        {
            get { return (byte)(x + 128); }
            set
            {
                if (value > 127)
                {
                    x = (sbyte)(value - 256);
                }
                else {
                    x = (sbyte)value;
                }
            }
        }

        public byte Ybyte
        {
            get { return (byte)(y + 128); }
            set
            {
                if (value > 127)
                {
                    y = (sbyte)(value - 256);
                }
                else
                {
                    y = (sbyte)value;
                }
            }
        }

        public byte Zbyte
        {
            get { return (byte)(z + 128); }
            set
            {
                if (value > 127)
                {
                    z = (sbyte)(value - 256);
                }
                else
                {
                    z = (sbyte)value;
                }
            }
        }

        public sbyte Z
        {
            get { return z; }
            set { z = value; }
        }

        public sbyte Y
        {
            get { return y; }
            set { y = value; }
        }

        public sbyte X
        {
            get { return x; }
            set { x = value; }
        }

        public override string ToString()
        {
            return "Orientamento: [Asse X: " + x + " Asse Y: " + y + " Asse Z: " + z + "]";
        }

        public static bool operator ==(Orientamento o1, Orientamento o2)
        {
            if (((object)o1) == ((object)o2)) return true;
            else if (((object)o1) == null || ((object)o2) == null) return false;
            else if ((o1.X == o2.X) && (o1.Y == o2.Y) && (o1.Z == o2.Z)) return true;
            else return false;
        }

        public bool checkEqual(Orientamento o2)
        {
            if (o2 != null)
            {
                return checkValue(o2.X, this.X) && checkValue(o2.Y, this.Y) && checkValue(o2.Z, this.Z);
            }
            return false;
        }

        public static bool operator !=(Orientamento o1, Orientamento o2)
        {
            return !(o1 == o2);
        }

        public override bool Equals(object obj)
        {
            if (obj is Orientamento)
            {
                return (this == obj);
            }
            return false;
        }

        private bool checkValue(float number1, float number2)
        {
            return (number1 < number2 + 25) && (number1 > number2 - 25);
        }
    }
}
