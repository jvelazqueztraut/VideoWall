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

  addPlayer(event, config) {
    config.players.push({
      id: config.players.length+1,
      zones : [],
      background:{ r: 0, g: 0, b: 0 },
      contents: []
    });
  }

  openContent(event,content) {
    this.navCtrl.push(ContentPage,{
      content: content
    });
  }

  addContent(event, player) {
    player.contents.push({
      type: "image",
      load: "",
      reload: false,
      loop: "none",
      time: 0,
      repetitions: 0
    });
    this.navCtrl.push(ContentPage,{
      content: player.contents[player.contents.length-1]
    });
  }
}
