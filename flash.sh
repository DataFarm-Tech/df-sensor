figlet "DataFarm"
cowsay "Hello $(whoami), you are running on ESP32"
sudo pio run 
sudo pio run --target upload -v
sudo pio device monitor
