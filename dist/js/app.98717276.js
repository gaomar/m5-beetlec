(function(t){function e(e){for(var i,o,l=e[0],c=e[1],u=e[2],f=0,p=[];f<l.length;f++)o=l[f],Object.prototype.hasOwnProperty.call(a,o)&&a[o]&&p.push(a[o][0]),a[o]=0;for(i in c)Object.prototype.hasOwnProperty.call(c,i)&&(t[i]=c[i]);s&&s(e);while(p.length)p.shift()();return r.push.apply(r,u||[]),n()}function n(){for(var t,e=0;e<r.length;e++){for(var n=r[e],i=!0,l=1;l<n.length;l++){var c=n[l];0!==a[c]&&(i=!1)}i&&(r.splice(e--,1),t=o(o.s=n[0]))}return t}var i={},a={app:0},r=[];function o(e){if(i[e])return i[e].exports;var n=i[e]={i:e,l:!1,exports:{}};return t[e].call(n.exports,n,n.exports,o),n.l=!0,n.exports}o.m=t,o.c=i,o.d=function(t,e,n){o.o(t,e)||Object.defineProperty(t,e,{enumerable:!0,get:n})},o.r=function(t){"undefined"!==typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(t,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(t,"__esModule",{value:!0})},o.t=function(t,e){if(1&e&&(t=o(t)),8&e)return t;if(4&e&&"object"===typeof t&&t&&t.__esModule)return t;var n=Object.create(null);if(o.r(n),Object.defineProperty(n,"default",{enumerable:!0,value:t}),2&e&&"string"!=typeof t)for(var i in t)o.d(n,i,function(e){return t[e]}.bind(null,i));return n},o.n=function(t){var e=t&&t.__esModule?function(){return t["default"]}:function(){return t};return o.d(e,"a",e),e},o.o=function(t,e){return Object.prototype.hasOwnProperty.call(t,e)},o.p="/";var l=window["webpackJsonp"]=window["webpackJsonp"]||[],c=l.push.bind(l);l.push=e,l=l.slice();for(var u=0;u<l.length;u++)e(l[u]);var s=c;r.push([0,"chunk-vendors"]),n()})({0:function(t,e,n){t.exports=n("56d7")},"56d7":function(t,e,n){"use strict";n.r(e);var i=n("2b0e"),a=function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("v-app",[n("v-content",[n("HelloWorld")],1)],1)},r=[],o=function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("v-container",[n("v-layout",{attrs:{"text-center":"",wrap:""}},[n("v-flex",{attrs:{"mb-4":""}},[n("h1",{staticClass:"display-2 font-weight-bold mb-3"},[t._v("\n        LINE Things BeetleC\n      ")]),n("h2",[t._v(t._s(t.status))]),n("h3",[t._v(t._s(t.bleStatus))])])],1)],1)},l=[],c={data(){return{status:"",USER_SERVICE_UUID:"119a679c-bb04-4798-8ff9-7ec96eb437ee",LED_CHARACTERISTIC_UUID:"E9062E71-9E62-4BC6-B0D3-35CDCD9B027B",bleConnect:!1,bleStatus:"デバイス未接続",characteristic:"",prevX:0,prevY:0}},created(){this.initJoyStick()},mounted:function(){liff.init(()=>this.initializeLiff())},methods:{liffCheckAvailablityAndDo:async function(t){try{const e=await liff.bluetooth.getAvailability();e?t():(this.bleStatus="Bluetoothをオンにしてください。",setTimeout(()=>this.liffCheckAvailablityAndDo(t),1e4))}catch(e){this.bleStatus="Bluetoothをオンにしてください。"}},liffRequestDevice:async function(){const t=await liff.bluetooth.requestDevice();await t.gatt.connect();const e=await t.gatt.getPrimaryService(this.USER_SERVICE_UUID);e.getCharacteristic(this.LED_CHARACTERISTIC_UUID).then(t=>{this.characteristic=t,this.bleConnect=!0,this.bleStatus="デバイスに接続しました！"}).catch(t=>{this.bleConnect=!0,this.bleStatus=`デバイス接続に失敗=${t.message}`})},initializeLiff:async function(){await liff.initPlugins(["bluetooth"]),this.liffCheckAvailablityAndDo(()=>this.liffRequestDevice())},initJoyStick(){var t=this,e=new VirtualJoystick({container:document.getElementById("container"),mouseSupport:!0,limitStickTravel:!0});e.addEventListener("touchStart",(function(){console.log("down")})),e.addEventListener("touchEnd",(function(){console.log("up")}));var n=0,i=0;setInterval((function(){if(n=Math.round(e.deltaX()),i=-1*Math.round(e.deltaY()),t.prevY!=i||t.prevX!=n){var a={x:n,y:i},r=JSON.stringify(a);t.characteristic.writeValue(new TextEncoder("ascii").encode(r)).catch(e=>{t.bleStatus=e.message})}t.prevX=n,t.prevY=i}),1/30*1e3)}}},u=c,s=n("2877"),f=n("6544"),p=n.n(f),h=n("a523"),v=n("0e8f"),d=n("a722"),b=Object(s["a"])(u,o,l,!1,null,null,null),y=b.exports;p()(b,{VContainer:h["a"],VFlex:v["a"],VLayout:d["a"]});var g={name:"App",components:{HelloWorld:y},data:()=>({})},S=g,m=n("7496"),w=n("a75b"),_=Object(s["a"])(S,a,r,!1,null,null,null),C=_.exports;p()(_,{VApp:m["a"],VContent:w["a"]});var E=n("f309");i["a"].use(E["a"]);var D=new E["a"]({icons:{iconfont:"mdi"}});i["a"].config.productionTip=!1,new i["a"]({vuetify:D,render:t=>t(C)}).$mount("#app")}});
//# sourceMappingURL=app.98717276.js.map