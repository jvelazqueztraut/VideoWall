import { Component } from '@angular/core';

import { NavParams, ViewController} from 'ionic-angular';

@Component({
  selector: 'page-color',
  templateUrl: 'color.html'
})
export class ColorPopover {
  color: any;

  colors = {
    white: {
      'r': "255",
      'g': "255",
      'b': "255"
    },
    tan: {
      'r': "249",
      'g': "241",
      'b': "228"
    },
    grey: {
      'r': "76",
      'g': "75",
      'b': "80"
    },
    black: {
      'r': "0",
      'g': "0",
      'b': "0"
    }
  };

  constructor(public navParams: NavParams, public viewCtrl: ViewController) {
    // If we navigated to this page, we will have an item available as a nav param
    this.color = this.navParams.get('color');
    console.log(this.color);
  }

  changeColor(c) {
    this.color.r = c.r;
    this.color.g = c.g;
    this.color.b = c.b;
  }

  getColor(color){
    return 'rgb('+color.r+','+color.g+','+color.b+')';
  }

  getComplementaryColor(color){
    let d = 0;
    
    // Counting the perceptive luminance - human eye favors green color...
    let a = 1 - ( 0.299 * +color.r + 0.587 * +color.g + 0.114 * +color.b)/255;
    
    if (a < 0.5)
        d = 0; // bright colors - black font
    else
        d = 255; // dark colors - white font
    
    return 'rgb('+d+','+d+','+d+')';
  }

  dismiss() {
    this.viewCtrl.dismiss();
  }
}
