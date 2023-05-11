//helper funs

const d = i => document.getElementById(i);
const ae = (o, e, f) => { o.addEventListener(e, f) };

//global variables
//error messages handler
var ermid = null;
const HOST = window.location.origin;
const TIMEOUT_DELAY = 5e3;
//validate credentials
const vdt = (sid, psd)=>{
    if(sid.length === 0 || psd.length === 0) return false;
    if(!sid.match(/^[^!#;+\]\/"\t][^+\]"\t]{0,31}$/)) return false;
    if(!psd.match(/^[\u0020-\u007e]{8,63}$/)) return false;
    return true;
}
//display error for 3s
const rerr = ()=>{
    const id = 'ID_ERR_MSG';
    const bid = 'ID_ERR_BG';
    const p_ = d(id);
    const b_ = d(bid);
    p_.innerText = "";
    b_.classList.remove('err_disp');
    ermid = null;
}
const derr = (m) =>{
    const id = 'ID_ERR_MSG';
    const bid = 'ID_ERR_BG';
    const p_ = d(id);
    const b_ = d(bid);
    //add error
    p_.innerText = m;
    if(ermid === null){
       
        b_.classList.add('err_disp');
        ermid = setTimeout(rerr, TIMEOUT_DELAY);
        return;
    }
    clearTimeout(ermid);
    ermid = setTimeout(rerr, TIMEOUT_DELAY);
}

//send credentials via POST

const __pf = (t_,s_,p_) => fetch(`${HOST}/update?type=${t_}`, {
    method: 'POST',
    headers: {
        'content-type' : 'application/json'
    },
    body : JSON.stringify(
        {ssid:s_,
        pass:p_}
    )
}).then(res=>{
    if(!res.ok){
        console.log(`HTTP error! Status : ${res.status}`);
        derr(`HTTP error! Status : ${res.status}`);
    }
    return res.text();
}).then(data=>console.log(data)
).catch(e=>{
    console.log(e);
    derr(e);
})

//submit credentials (e)
const s_creds = (e) => {
    const __ = e.target.id.substring(3);
    const t_ = __.substring(0, __.indexOf('_'));
    const ssid_ = `ID_${t_}_SSID`;
    const pass_ = `ID_${t_}_PASS`;
    const s_t = d(ssid_);
    const p_t = d(pass_)
    const ssid = s_t.value;
    const pass = p_t.value;
    s_t.value= "";
    p_t.value= "";
    if(!vdt(ssid, pass)){
        derr(`The ${t_} credentials are invalid`);
        return;
    }
    console.log(ssid, pass);
    __pf(t_, ssid, pass);
}

//ready
ae(document, 'DOMContentLoaded', () => {
    ae(d('ID_STA_SUBMIT'), 'click', s_creds);
    ae(d('ID_AP_SUBMIT'), 'click', s_creds);
})






