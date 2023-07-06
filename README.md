# Adafruit on Bela
This repository is trying to provide an example of how to use common sensors from Adafruit connected via I2C (maybe also SPI in the future) on the Bela platform

## WIP
The implementation is not done. But you can check the [analysis](analysis.md), which describes which steps we already performed and tested.

# SYNC

```shell
rsync -ar -P --exclude 'LICENSE*' --exclude 'library.properties' --exclude '.git*' --exclude '*.md' --exclude 'pics' --exclude 'examples' --delete * root@bela.local:~/Bela/projects/i2c_test
```
