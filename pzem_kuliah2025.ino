#include <PZEM004Tv30.h>

// Gunakan Serial2 (software serial) karena Serial0 digunakan untuk debug
#include <SoftwareSerial.h>
SoftwareSerial pzemSerial(D6, D7); // RX, TX → sesuaikan pin!

PZEM004Tv30 pzem(pzemSerial);

void setup() {
  Serial.begin(115200);
  pzemSerial.begin(9600);
  
  // Opsional: atur alamat Modbus (default 0xF8)
  pzem.setAddress(0x01);
  
  Serial.println("Mengukur daya dengan PZEM-004T...");
}

void loop() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  if(isnan(voltage)){
    Serial.println("Error membaca PZEM!");
  } else {
    Serial.print("Tegangan: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Arus: "); Serial.print(current); Serial.println(" A");
    Serial.print("Daya: "); Serial.print(power); Serial.println(" W");
    Serial.print("Energi: "); Serial.print(energy); Serial.println(" kWh");
    Serial.print("Frekuensi: "); Serial.print(frequency); Serial.println(" Hz");
    Serial.print("PF: "); Serial.println(pf);
  }

  delay(2000);
}
