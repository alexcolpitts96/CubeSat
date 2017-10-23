void setup() {
  Serial.begin(9600);
  while(!Serial){;}; // ensure that serial is setup properly 

  Serial.println("433 MHz UART Test Begin");
}

int i = 0;

void loop() {
  Serial.println(i);
  delay(50);

  i++;
}
