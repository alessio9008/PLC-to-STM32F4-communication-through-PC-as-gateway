using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CodeProjectSerialComms
{


    public partial class PLCState : Form
    {

        private float counter = 0;
        private float number = 0;
        private Param param;
        
        private Image image;
        private bool enabledMove = true;

        private object lockposition = new object();
        private object lockangle = new object();

        private Queue<State> plcToGUI;

        public PLCState()
        {
            plcToGUI = new Queue<State>();
            init();
        }

        public void addLast(State s) {
            if(s!=plcToGUI.Tail)plcToGUI.Waitinsert(s);
        }

        private void init() {
            param = Param.getInstance();
            InitializeComponent();
            this.Bounds = new Rectangle(new Point(0, 0), this.Size);
            image = Properties.Resources.Braccio1big;
            TmPictureBoxImage.Interval = param.frequencyRefreshImage;
        }

        
        public void setText(string text)
        {
            this.richPLCOutput.Text = text + this.richPLCOutput.Text;
        }

        public void setTextln(string text)
        {
            this.setText(text + "\n");
        }

        private void rotateAnimation(float i)
        {
            pbMove.Image = RotateImage(image, new PointF((float)image.Width / 2, (float)image.Height / 2), i);
            pbMove.Refresh();
        }

        public static Bitmap RotateImage(Image image, PointF offset, float angle)
        {
            if (image == null)
                throw new ArgumentNullException("image");

            //create a new empty bitmap to hold rotated image
            Bitmap rotatedBmp = new Bitmap(image.Width, image.Height);
            rotatedBmp.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            //make a graphics object from the empty bitmap
            Graphics g = Graphics.FromImage(rotatedBmp);

            //Put the rotation point in the center of the image
            g.TranslateTransform(offset.X, offset.Y);

            //rotate the image
            g.RotateTransform(angle);

            //move the image back
            g.TranslateTransform(-offset.X, -offset.Y);

            //draw passed in image onto graphics object
            g.DrawImage(image, new PointF(0, 0));

            return rotatedBmp;
        }

        private bool checkValue(float number1, float number2) {
            return (number1 < number2 + param.minAngleRotate) && (number1 > number2 - param.minAngleRotate);
        }

        private void TmPictureBoxImage_Tick(object sender, EventArgs e)
        {

            if (checkValue(counter, number) || !chdoNotWaitComplete.Checked) {
                State item = plcToGUI.TryRemove();
                if (item != null) number = (item.StandardPosition + item.Angle) % 361;
            }
            if (counter < number)
            {
                rotateAnimation(counter);
                counter = (counter + param.minAngleRotate)%361;

            }
            else if (counter > number)
            {
                rotateAnimation(counter);
                counter = (counter - param.minAngleRotate) % 361;
            }
            
        }

        private void btnPLCClear_Click(object sender, EventArgs e)
        {
            richPLCOutput.Clear();
            richPLCOutput.Text = string.Empty;
        }



    }
}
