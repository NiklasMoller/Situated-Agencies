var counter = 0;

function setup(){
 createCanvas(400, 400);
 textSize(32);


}

function draw(){

}


function mqttRecieved(data){

  clear();

  switch(counter) {
    case 0:
      // code block

      data = 100 - data;

      if(data > 0){
        var lightness = 'hsl(169, 16%,' + data + '%)';

        c = color(lightness);
        background(c);
      }


      break;
    case 1:

      text(data, 10, 30);
      break;
    default:
      // code block
  }




}


function mouseClicked(){
  counter++;
  counter = counter % 5;
}
