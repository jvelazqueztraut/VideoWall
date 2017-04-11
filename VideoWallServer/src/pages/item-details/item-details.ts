import { Component } from '@angular/core';

import { NavController, NavParams, ModalController } from 'ionic-angular';

import { ContentPage } from '../content/content';


@Component({
  selector: 'page-item-details',
  templateUrl: 'item-details.html'
})
export class ItemDetailsPage {
  selectedConfig: any;

  constructor(public navCtrl: NavController, public navParams: NavParams, public modalCtrl: ModalController) {
    // If we navigated to this page, we will have an item available as a nav param
    this.selectedConfig = this.navParams.get('config');
  }

  openModal(event,content) {
    this.navCtrl.push(ContentPage,{
      content: content
    });
  }
}
