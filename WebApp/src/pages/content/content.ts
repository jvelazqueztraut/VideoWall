import { Component } from '@angular/core';

import { NavController, NavParams, ViewController} from 'ionic-angular';

@Component({
  selector: 'page-content',
  templateUrl: 'content.html'
})
export class ContentPage {
  selectedContent: any;
  date: string;

  constructor(public navCtrl: NavController, public navParams: NavParams, public viewCtrl: ViewController) {
    // If we navigated to this page, we will have an item available as a nav param
    this.selectedContent = this.navParams.get('content');
    this.setDate();
  }

  setDate(){
    let d = this.selectedContent.date;
    console.log(d);
    let dd = new Date(Date.UTC(d.year, d.month-1, d.day, d.hour, d.minute, 0, 0));
    this.date = dd.toISOString();
  }

  parseDate(event){
    console.log(event);
    let dd = new Date(event.year.value, event.month.value, event.day.value, event.hour.value, event.minute.value, 0, 0);
    this.selectedContent.date.year = dd.getFullYear();
    this.selectedContent.date.month = dd.getMonth();
    this.selectedContent.date.day = dd.getDate();
    this.selectedContent.date.hour = dd.getHours();
    this.selectedContent.date.minute = dd.getMinutes();
    //this.setDate();
  }

  dismiss() {
    this.viewCtrl.dismiss();
  }
}
