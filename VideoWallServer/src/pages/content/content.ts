import { Component } from '@angular/core';

import { NavController, NavParams, ViewController} from 'ionic-angular';

@Component({
  selector: 'page-content',
  templateUrl: 'content.html'
})
export class ContentPage {
  selectedContent: any;

  constructor(public navCtrl: NavController, public navParams: NavParams) {
    // If we navigated to this page, we will have an item available as a nav param
    this.selectedContent = this.navParams.get('content');
  }
}
