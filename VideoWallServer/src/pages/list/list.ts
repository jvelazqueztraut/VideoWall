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
    background: { r: number, g: number, b: number },
    players: Array<{
      id: number,
      zones: number[],
      background:{ r: number, g: number, b: number },
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
        background: { r: 0, g: 0, b: 0 },
        players: [ {
          id: 1,
          zones : [0,1,2],
          background:{ r: 255, g: 255, b: 255 },
          contents: [ {
            type: "image",
            load: "test.png",
            reload: false,
            loop: "time",
            loopParameter: 10
          },
          {
            type: "image",
            load: "test.png",
            reload: false,
            loop: "time",
            loopParameter: 10
          } ] 
        },
        {
          id: 2,
          zones : [4,5,7,8],
          background:{ r: 255, g: 255, b: 255 },
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
