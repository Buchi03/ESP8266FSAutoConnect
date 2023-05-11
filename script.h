const char _script[] PROGMEM = R"=====(
const d=e=>document.getElementById(e),ae=(e,r,t)=>{e.addEventListener(r,t)};var ermid=null;const HOST=window.location.origin,TIMEOUT_DELAY=5e3,vdt=(e,r)=>!!(0!==e.length&&0!==r.length&&e.match(/^[^!#;+\]\/"\t][^+\]"\t]{0,31}$/)&&r.match(/^[\u0020-\u007e]{8,63}$/)),rerr=()=>{let e=d("ID_ERR_MSG"),r=d("ID_ERR_BG");e.innerText="",r.classList.remove("err_disp"),ermid=null},derr=e=>{let r=d("ID_ERR_MSG"),t=d("ID_ERR_BG");if(r.innerText=e,null===ermid){t.classList.add("err_disp"),ermid=setTimeout(rerr,5e3);return}clearTimeout(ermid),ermid=setTimeout(rerr,5e3)},__pf=(e,r,t)=>fetch(`${HOST}/update?type=${e}`,{method:"POST",headers:{"content-type":"application/json"},body:JSON.stringify({ssid:r,pass:t})}).then(e=>(e.ok||(console.log(`HTTP error! Status : ${e.status}`),derr(`HTTP error! Status : ${e.status}`)),e.text())).then(e=>console.log(e)).catch(e=>{console.log(e),derr(e)}),s_creds=e=>{let r=e.target.id.substring(3),t=r.substring(0,r.indexOf("_")),s=`ID_${t}_SSID`,i=`ID_${t}_PASS`,n=d(s),a=d(i),l=n.value,o=a.value;if(n.value="",a.value="",!vdt(l,o)){derr(`The ${t} credentials are invalid`);return}console.log(l,o),__pf(t,l,o)};ae(document,"DOMContentLoaded",()=>{ae(d("ID_STA_SUBMIT"),"click",s_creds),ae(d("ID_AP_SUBMIT"),"click",s_creds)});
)=====";