namespace _3031Project
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            startbtn = new Button();
            stopbtn = new Button();
            comboBox1 = new ComboBox();
            motor1lbl = new Label();
            motor2lbl = new Label();
            motor3lbl = new Label();
            gripper1lbl = new Label();
            m1vbx = new TextBox();
            m1pbx = new TextBox();
            m2pbx = new TextBox();
            m2vbx = new TextBox();
            label1 = new Label();
            label2 = new Label();
            m3pbx = new TextBox();
            m3vbx = new TextBox();
            m1fBtn = new Button();
            m1rBtn = new Button();
            m2fBtn = new Button();
            m2rBtn = new Button();
            m3fBtn = new Button();
            m3vBtn = new Button();
            gfBtn = new Button();
            glBtn = new Button();
            grBtn = new Button();
            gbBtn = new Button();
            guBtn = new Button();
            gdBtn = new Button();
            diagBtn = new Button();
            obstBtn = new Button();
            homeBtn = new Button();
            dp1Bx = new TextBox();
            dp2Bx = new TextBox();
            dp3Bx = new TextBox();
            label3 = new Label();
            SuspendLayout();
            // 
            // startbtn
            // 
            startbtn.Location = new Point(12, 12);
            startbtn.Name = "startbtn";
            startbtn.Size = new Size(112, 34);
            startbtn.TabIndex = 0;
            startbtn.Text = "Initialize";
            startbtn.UseVisualStyleBackColor = true;
            startbtn.Click += startbtn_Click;
            // 
            // stopbtn
            // 
            stopbtn.Location = new Point(130, 12);
            stopbtn.Name = "stopbtn";
            stopbtn.Size = new Size(112, 34);
            stopbtn.TabIndex = 1;
            stopbtn.Text = "Abort";
            stopbtn.UseVisualStyleBackColor = true;
            stopbtn.Click += stopbtn_Click;
            // 
            // comboBox1
            // 
            comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBox1.FormattingEnabled = true;
            comboBox1.Location = new Point(248, 12);
            comboBox1.Name = "comboBox1";
            comboBox1.Size = new Size(182, 33);
            comboBox1.TabIndex = 2;
            // 
            // motor1lbl
            // 
            motor1lbl.AutoSize = true;
            motor1lbl.Location = new Point(130, 72);
            motor1lbl.Name = "motor1lbl";
            motor1lbl.Size = new Size(77, 25);
            motor1lbl.TabIndex = 3;
            motor1lbl.Text = "Motor 1";
            // 
            // motor2lbl
            // 
            motor2lbl.AutoSize = true;
            motor2lbl.Location = new Point(304, 72);
            motor2lbl.Name = "motor2lbl";
            motor2lbl.Size = new Size(77, 25);
            motor2lbl.TabIndex = 4;
            motor2lbl.Text = "Motor 2";
            // 
            // motor3lbl
            // 
            motor3lbl.AutoSize = true;
            motor3lbl.Location = new Point(476, 72);
            motor3lbl.Name = "motor3lbl";
            motor3lbl.Size = new Size(77, 25);
            motor3lbl.TabIndex = 5;
            motor3lbl.Text = "Motor 3";
            // 
            // gripper1lbl
            // 
            gripper1lbl.AutoSize = true;
            gripper1lbl.Location = new Point(717, 72);
            gripper1lbl.Name = "gripper1lbl";
            gripper1lbl.Size = new Size(71, 25);
            gripper1lbl.TabIndex = 6;
            gripper1lbl.Text = "Gripper";
            // 
            // m1vbx
            // 
            m1vbx.Location = new Point(92, 184);
            m1vbx.Name = "m1vbx";
            m1vbx.ReadOnly = true;
            m1vbx.Size = new Size(150, 31);
            m1vbx.TabIndex = 7;
            // 
            // m1pbx
            // 
            m1pbx.Location = new Point(92, 111);
            m1pbx.Name = "m1pbx";
            m1pbx.ReadOnly = true;
            m1pbx.Size = new Size(150, 31);
            m1pbx.TabIndex = 8;
            // 
            // m2pbx
            // 
            m2pbx.Location = new Point(268, 111);
            m2pbx.Name = "m2pbx";
            m2pbx.ReadOnly = true;
            m2pbx.Size = new Size(150, 31);
            m2pbx.TabIndex = 9;
            // 
            // m2vbx
            // 
            m2vbx.Location = new Point(268, 184);
            m2vbx.Name = "m2vbx";
            m2vbx.ReadOnly = true;
            m2vbx.Size = new Size(150, 31);
            m2vbx.TabIndex = 10;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 111);
            label1.Name = "label1";
            label1.Size = new Size(75, 25);
            label1.TabIndex = 11;
            label1.Text = "Position";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 190);
            label2.Name = "label2";
            label2.Size = new Size(73, 25);
            label2.TabIndex = 12;
            label2.Text = "Velocity";
            // 
            // m3pbx
            // 
            m3pbx.Location = new Point(442, 111);
            m3pbx.Name = "m3pbx";
            m3pbx.ReadOnly = true;
            m3pbx.Size = new Size(150, 31);
            m3pbx.TabIndex = 16;
            // 
            // m3vbx
            // 
            m3vbx.Location = new Point(442, 184);
            m3vbx.Name = "m3vbx";
            m3vbx.ReadOnly = true;
            m3vbx.Size = new Size(150, 31);
            m3vbx.TabIndex = 17;
            // 
            // m1fBtn
            // 
            m1fBtn.Location = new Point(112, 241);
            m1fBtn.Name = "m1fBtn";
            m1fBtn.Size = new Size(112, 34);
            m1fBtn.TabIndex = 18;
            m1fBtn.Text = "Forward";
            m1fBtn.UseVisualStyleBackColor = true;
            m1fBtn.Click += m1fBtn_Click;
            // 
            // m1rBtn
            // 
            m1rBtn.Location = new Point(112, 281);
            m1rBtn.Name = "m1rBtn";
            m1rBtn.Size = new Size(112, 34);
            m1rBtn.TabIndex = 19;
            m1rBtn.Text = "Reverse";
            m1rBtn.UseVisualStyleBackColor = true;
            m1rBtn.Click += m1rBtn_Click;
            // 
            // m2fBtn
            // 
            m2fBtn.Location = new Point(286, 241);
            m2fBtn.Name = "m2fBtn";
            m2fBtn.Size = new Size(112, 34);
            m2fBtn.TabIndex = 20;
            m2fBtn.Text = "Forward";
            m2fBtn.UseVisualStyleBackColor = true;
            m2fBtn.Click += m2fBtn_Click;
            // 
            // m2rBtn
            // 
            m2rBtn.Location = new Point(286, 281);
            m2rBtn.Name = "m2rBtn";
            m2rBtn.Size = new Size(112, 34);
            m2rBtn.TabIndex = 21;
            m2rBtn.Text = "Reverse";
            m2rBtn.UseVisualStyleBackColor = true;
            m2rBtn.Click += m2rBtn_Click;
            // 
            // m3fBtn
            // 
            m3fBtn.Location = new Point(458, 241);
            m3fBtn.Name = "m3fBtn";
            m3fBtn.Size = new Size(112, 34);
            m3fBtn.TabIndex = 22;
            m3fBtn.Text = "Forward";
            m3fBtn.UseVisualStyleBackColor = true;
            m3fBtn.Click += m3fBtn_Click;
            // 
            // m3vBtn
            // 
            m3vBtn.Location = new Point(458, 281);
            m3vBtn.Name = "m3vBtn";
            m3vBtn.Size = new Size(112, 34);
            m3vBtn.TabIndex = 23;
            m3vBtn.Text = "Reverse";
            m3vBtn.UseVisualStyleBackColor = true;
            m3vBtn.Click += m3vBtn_Click;
            // 
            // gfBtn
            // 
            gfBtn.Font = new Font("Segoe UI", 11F);
            gfBtn.Location = new Point(714, 111);
            gfBtn.Name = "gfBtn";
            gfBtn.Size = new Size(74, 49);
            gfBtn.TabIndex = 24;
            gfBtn.Text = "↑";
            gfBtn.UseVisualStyleBackColor = true;
            // 
            // glBtn
            // 
            glBtn.Font = new Font("Segoe UI", 11F);
            glBtn.Location = new Point(635, 166);
            glBtn.Name = "glBtn";
            glBtn.Size = new Size(74, 49);
            glBtn.TabIndex = 25;
            glBtn.Text = "←";
            glBtn.UseVisualStyleBackColor = true;
            // 
            // grBtn
            // 
            grBtn.Font = new Font("Segoe UI", 11F);
            grBtn.Location = new Point(794, 166);
            grBtn.Name = "grBtn";
            grBtn.Size = new Size(74, 49);
            grBtn.TabIndex = 26;
            grBtn.Text = "→";
            grBtn.UseVisualStyleBackColor = true;
            // 
            // gbBtn
            // 
            gbBtn.Font = new Font("Segoe UI", 11F);
            gbBtn.Location = new Point(714, 166);
            gbBtn.Name = "gbBtn";
            gbBtn.Size = new Size(74, 49);
            gbBtn.TabIndex = 27;
            gbBtn.Text = "↓";
            gbBtn.UseVisualStyleBackColor = true;
            // 
            // guBtn
            // 
            guBtn.Font = new Font("Segoe UI", 9F);
            guBtn.Location = new Point(634, 111);
            guBtn.Name = "guBtn";
            guBtn.Size = new Size(74, 49);
            guBtn.TabIndex = 28;
            guBtn.Text = "Up";
            guBtn.UseVisualStyleBackColor = true;
            // 
            // gdBtn
            // 
            gdBtn.Font = new Font("Segoe UI", 9F);
            gdBtn.Location = new Point(794, 111);
            gdBtn.Name = "gdBtn";
            gdBtn.Size = new Size(74, 49);
            gdBtn.TabIndex = 29;
            gdBtn.Text = "Down";
            gdBtn.UseVisualStyleBackColor = true;
            // 
            // diagBtn
            // 
            diagBtn.Location = new Point(621, 241);
            diagBtn.Name = "diagBtn";
            diagBtn.Size = new Size(152, 34);
            diagBtn.TabIndex = 33;
            diagBtn.Text = "Diagonal Line";
            diagBtn.UseVisualStyleBackColor = true;
            diagBtn.Click += diagBtn_Click;
            // 
            // obstBtn
            // 
            obstBtn.Location = new Point(621, 281);
            obstBtn.Name = "obstBtn";
            obstBtn.Size = new Size(152, 34);
            obstBtn.TabIndex = 34;
            obstBtn.Text = "Traverse Wall";
            obstBtn.UseVisualStyleBackColor = true;
            obstBtn.Click += obstBtn_Click;
            // 
            // homeBtn
            // 
            homeBtn.Location = new Point(779, 241);
            homeBtn.Name = "homeBtn";
            homeBtn.Size = new Size(89, 74);
            homeBtn.TabIndex = 35;
            homeBtn.Text = "Home";
            homeBtn.UseVisualStyleBackColor = true;
            homeBtn.Click += homeBtn_Click;
            // 
            // dp1Bx
            // 
            dp1Bx.Location = new Point(92, 148);
            dp1Bx.Name = "dp1Bx";
            dp1Bx.Size = new Size(150, 31);
            dp1Bx.TabIndex = 36;
            dp1Bx.DoubleClick += dp1Bx_DoubleClick;
            // 
            // dp2Bx
            // 
            dp2Bx.Location = new Point(268, 148);
            dp2Bx.Name = "dp2Bx";
            dp2Bx.Size = new Size(150, 31);
            dp2Bx.TabIndex = 37;
            dp2Bx.DoubleClick += dp2Bx_DoubleClick;
            // 
            // dp3Bx
            // 
            dp3Bx.Location = new Point(442, 147);
            dp3Bx.Name = "dp3Bx";
            dp3Bx.Size = new Size(150, 31);
            dp3Bx.TabIndex = 38;
            dp3Bx.DoubleClick += dp3Bx_DoubleClick;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(12, 151);
            label3.Name = "label3";
            label3.Size = new Size(62, 25);
            label3.TabIndex = 39;
            label3.Text = "D. Pos";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(888, 367);
            Controls.Add(label3);
            Controls.Add(dp3Bx);
            Controls.Add(dp2Bx);
            Controls.Add(dp1Bx);
            Controls.Add(homeBtn);
            Controls.Add(obstBtn);
            Controls.Add(diagBtn);
            Controls.Add(gdBtn);
            Controls.Add(guBtn);
            Controls.Add(gbBtn);
            Controls.Add(grBtn);
            Controls.Add(glBtn);
            Controls.Add(gfBtn);
            Controls.Add(m3vBtn);
            Controls.Add(m3fBtn);
            Controls.Add(m2rBtn);
            Controls.Add(m2fBtn);
            Controls.Add(m1rBtn);
            Controls.Add(m1fBtn);
            Controls.Add(m3vbx);
            Controls.Add(m3pbx);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(m2vbx);
            Controls.Add(m2pbx);
            Controls.Add(m1pbx);
            Controls.Add(m1vbx);
            Controls.Add(gripper1lbl);
            Controls.Add(motor3lbl);
            Controls.Add(motor2lbl);
            Controls.Add(motor1lbl);
            Controls.Add(comboBox1);
            Controls.Add(stopbtn);
            Controls.Add(startbtn);
            Name = "Form1";
            Text = "SCORBOT Control";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button startbtn;
        private Button stopbtn;
        private ComboBox comboBox1;
        private Label motor1lbl;
        private Label motor2lbl;
        private Label motor3lbl;
        private Label gripper1lbl;
        private TextBox m1vbx;
        private TextBox m1pbx;
        private TextBox m2pbx;
        private TextBox m2vbx;
        private TextBox m3pbx;
        private Label label1;
        private Label label2;
        private TextBox dp1Bx;
        private TextBox dp2Bx;
        private TextBox dp3Bx;
        private TextBox m3vbx;
        private TextBox textBox5;
        private Button m1fBtn;
        private Button m1rBtn;
        private Button m2fBtn;
        private Button m2rBtn;
        private Button m3fBtn;
        private Button m3vBtn;
        private Button gfBtn;
        private Button glBtn;
        private Button grBtn;
        private Button gbBtn;
        private Button guBtn;
        private Button gdBtn;
        private Button diagBtn;
        private Button obstBtn;
        private Button homeBtn;
        private Label label3;
    }
}
