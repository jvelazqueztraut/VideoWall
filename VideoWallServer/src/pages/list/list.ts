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
    zones: number[],
    players: Array<{
      id: number,
      background:{ r: number, g: number, b: number },
      contents: Array<{
        type: string,
        load: string,
        reload: boolean,
        loop: string,
        time: number,
        repetitions: number
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
        zones: [ 0, 0, 0, 0, 0, 0, 0 , 0, 0 ],
        players: [ {
          id: 0,
          background:{ r: 255, g: 255, b: 255 },
          contents: [ {
            type: "image",
            load: "test.png",
            reload: false,
            loop: "time",
            time: 10,
            repetitions: 5
          },
          {
            type: "image",
            load: "test.png",
            reload: false,
            loop: "time",
            time: 10,
            repetitions: 5
          } ] 
        },
        {
          id: 1,
          background:{ r: 255, g: 255, b: 255 },
          contents: [ {
            type: "image",
            load: "test.png",
            reload: false,
            loop: "time",
            time: 10,
            repetitions: 5
          } ]
        } ]
      });
    }
  }

  openConfig(event, config) {
    this.navCtrl.push(ItemDetailsPage, {
      config: config
    });
  }
}
