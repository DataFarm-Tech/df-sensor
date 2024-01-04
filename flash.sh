figlet "DataFarm"
cowsay "Hello $(whoami), you are running on ESP32"
sudo date >> /logs/pio.log
sudo echo "Flashed by: $(whoami)" >> /logs/pio.log
sudo pio run 2>&1 | tee -a /logs/pio.log
sudo pio run --target upload -v 2>&1 | tee -a /logs/pio.log
sudo pio device monitor 2>&1 | tee -a /logs/pio.log
