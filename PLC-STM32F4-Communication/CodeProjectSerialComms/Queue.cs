using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace CodeProjectSerialComms
{
    public class Queue<T> : LinkedList<T>
    {
        public Queue() :base(){}

        public void Waitinsert(T item) { 
            lock(this){
                while (base.Count == int.MaxValue) Monitor.Wait(this);
                this.AddLast(item);
                Monitor.PulseAll(this);
            }
        }

        public T Waitremove() {
            T item;
            lock (this) {
                while (base.Count == 0) Monitor.Wait(this);
                item = base.First.Value;
                base.RemoveFirst();
                Monitor.PulseAll(this);
            }
            return item;
        }

        public T TryRemove() {
            T item=default(T);
            lock (this) {
                if (base.Count > 0) {
                    item = base.First.Value;
                    base.RemoveFirst();
                }
            }
            return item;
        }

        public T Tail{
            get {
                if (base.Count > 0) return base.Last.Value;
                return default(T);
            }
        }


        public T Head
        {
            get
            {
                if (base.Count > 0) return base.First.Value;
                return default(T);
            }
        }
    }
}
