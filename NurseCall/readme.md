# Sistem Graphical User Interface untuk NurseCall Analog 

**Overview**
Alat digunakan untuk membaca masukan dari sistem nursecall analog yang sudah ada. Pembacaan digunakan untuk menampilkan nomor kamar pada monitor dan melakukan panggilan pada audio. Secara hardware, sistem terdiri dari 
[NurseCall -> Hardware Interface -> SBC -> Monitor].

**Fitur Alat**
1. Membaca sinyal dari sistem NurseCall 0-24V sebanyak 36 Input biasa + 11 Emergency secara parallel
2. Isolasi sinyal input dari sistem utama 5V
3. Mengkonversi bacaan menjadi data digital yang dapat dibaca Single Board Computer.

*Sketsa Hubungan antara sistem monitor yang dibuat dengan NurseCall*
![Sketsa](https://github.com/mrproffirman/Portofolio/blob/main/NurseCall/img/Sketsa.jpg)

*Sirkuit Hardware Interface menggunakan Arduino Nano*
![Sirkuit](https://github.com/mrproffirman/Portofolio/blob/main/NurseCall/img/Circuit_Serial_3_color.png)

*GUI yang Dihasilkan*
![GUI](https://github.com/mrproffirman/Portofolio/blob/main/NurseCall/img/GUI.jpg)

*Hardware Interface*
![Hardware_Interface](https://github.com/mrproffirman/Portofolio/blob/main/NurseCall/img/Hardware%2BSBC.jpg)
![Hardware_Interface](https://github.com/mrproffirman/Portofolio/blob/main/NurseCall/img/Implementasi.jpg)
