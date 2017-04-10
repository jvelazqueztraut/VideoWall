import { Component } from '@angular/core';

import { NavController, NavParams } from 'ionic-angular';

import { ItemDetailsPage } from '../item-details/item-details';

@Component({
  selector: 'page-list',
  templateUrl: 'list.html'
})
export class ListPage {
  configurations: Array<{
    title: string, 
    note: string,
    width: number,
    height: number,
    fullscreen: boolean,
    framerate: number,
    background: { red: number, green: number, blue: number },
    players: Array<{
      width: number,
      height: number,
      x: number,
      y: number,
      background:{ red: number, green: number, blue: number },
      contents: Array<{
        type: string,
        load: string,
        reload: boolean,
        loop: string,
        loopParameter: number
      }>
    }>
  }>;

  constructor(public navCtrl: NavController, public navParams: NavParams) {
    this.configurations = [];
    for(let i = 0; i < 2; i++) {
      this.configurations.push({
        title: 'Configuration ' + i,
        note: 'This is configuration #' + i,
        width: 1920,
        height: 1080,
        fullscreen: true,
        framerate: 60,
        background: { red: 0, green: 0, blue: 0 },
        players: [ {
          width: 500,
          height: 300,
          x: 0,
          y: 0,
          background:{ red: 255, green: 255, blue: 255 },
          contents: [ {
            type: "image",
            load: "test.png",
            reload: false,
            loop: "time",
            loopParameter: 10
          } ]
        } ]
      });
    }
  }

  configTapped(event, config) {
    this.navCtrl.push(ItemDetailsPage, {
      config: config
    });
  }
}
