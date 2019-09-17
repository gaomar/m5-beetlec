<template>
  <v-container>
    <v-layout
      text-center
      wrap
    >
      <v-flex mb-4>
        <h1 class="display-2 font-weight-bold mb-3">
          LINE Things BeetleC
        </h1>
        <h3>{{bleStatus}}</h3>
      </v-flex>

    </v-layout>
  </v-container>
</template>

<script>
  export default {
    data () {
      return {
        status: '',
        USER_SERVICE_UUID: process.env.VUE_APP_USER_SERVICE_UUID,
        LED_CHARACTERISTIC_UUID: 'E9062E71-9E62-4BC6-B0D3-35CDCD9B027B',
        bleConnect: false,
        bleStatus: 'デバイス未接続',
        characteristic: '',
        prevX: 0,
        prevY: 0
      }
    },
    created () {
      this.initJoyStick()
    },
    mounted: function () {
      liff.init(
          () => this.initializeLiff()
      )
    },    
    methods: {
      led (value) {
        console.log(value)
        this.status = value

        var param = {
          x: 0,
          y: 0,
          led: value
        }
        var jsonStr = JSON.stringify(param)
        self.characteristic.writeValue(new TextEncoder('ascii').encode(jsonStr)
        ).catch(error => {
          self.bleStatus = error.message
        })

      },
      // BLEが接続できる状態になるまでリトライ
      liffCheckAvailablityAndDo: async function (callbackIfAvailable) {
        try {
          const isAvailable = await liff.bluetooth.getAvailability();
          if (isAvailable) {
            callbackIfAvailable()
          } else {
            // リトライ
            this.bleStatus = `Bluetoothをオンにしてください。`
            setTimeout(() => this.liffCheckAvailablityAndDo(callbackIfAvailable), 10000)
          }
        } catch (error) {
          this.bleStatus = `Bluetoothをオンにしてください。`
        }
      },
      // サービスとキャラクタリスティックにアクセス
      liffRequestDevice: async function () {
        const device = await liff.bluetooth.requestDevice()
        await device.gatt.connect()
        const service = await device.gatt.getPrimaryService(this.USER_SERVICE_UUID)
        service.getCharacteristic(this.LED_CHARACTERISTIC_UUID).then(characteristic => {
          this.characteristic = characteristic
          this.bleConnect = true
          this.bleStatus = `デバイスに接続しました！`
        }).catch(error => {
          this.bleConnect = true
          this.bleStatus = `デバイス接続に失敗=${error.message}`
        })
      },
      initializeLiff: async function(){
        await liff.initPlugins(['bluetooth']);
        this.liffCheckAvailablityAndDo(() => this.liffRequestDevice())
      },      
      initJoyStick () {
        var self = this

        var joystick	= new VirtualJoystick({
          container	: document.getElementById('container'),
          mouseSupport	: true,
          limitStickTravel	: true,
        });
        joystick.addEventListener('touchStart', function(){
          console.log('down')
        })
        joystick.addEventListener('touchEnd', function(){
          console.log('up')
        })

        var newX = 0
        var newY = 0
        var led = 0

        setInterval(function() {
          newX = Math.round(joystick.deltaX());
          newY = Math.round(joystick.deltaY()) * -1
          if (newY > 0) {
            led = 2
          } else if (newY < 0) {
            led = 3
          } else {
            led = 0
          }
          if (self.prevY != newY) {
            var param = {
              x: newX,
              y: newY,
              led: led
            }
            //self.characteristic.writeValue(new TextEncoder('ascii').encode(String(newY))
            var jsonStr = JSON.stringify(param)
            self.characteristic.writeValue(new TextEncoder('ascii').encode(jsonStr)
            ).catch(error => {
              self.bleStatus = error.message
            })
          }

          self.prevX = newX;
          self.prevY = newY;

        }, 1/30 * 1000);
              
      }
    }
  };
</script>
