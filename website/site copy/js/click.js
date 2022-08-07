var myImage = document.querySelector('img');
var myButton = document.querySelector('button');

myButton.onclick = function() {
    var mySrc = myImage.getAttribute('src');
    if(mySrc === 'img/banner/dog.png') {
      myImage.setAttribute ('src', "");
    } else {
      myImage.setAttribute ('src','img/banner/dog.png');
    }
}