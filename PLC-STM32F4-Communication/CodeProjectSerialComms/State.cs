using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeProjectSerialComms
{
    public class State
    {
        private float angle = 0;
        private int standardPosition = 0;
        public State(float ang, int pos) {
            this.angle = ang;
            this.standardPosition = pos;
        }

        public State() { }



        public float Angle
        {
            get { return angle; }
            set { angle = value; }
        }


        public int StandardPosition
        {
            get { return standardPosition; }
            set { standardPosition = value; }
        }

        public static bool operator ==(State s1, State s2)
        {
            if (((object)s1) == ((object)s2)) return true;
            else if (((object)s1) == null || ((object)s2) == null) return false;
            else if ((s1.standardPosition == s2.StandardPosition) && (s1.Angle == s2.Angle)) return true;
            else return false;
        }

        public static bool operator !=(State s1, State s2)
        {
            return !(s1 == s2);
        }

        public override bool Equals(object obj)
        {
            if (obj is State)
            {
                return (this == obj);
            }
            return false;
        }
    }
}
