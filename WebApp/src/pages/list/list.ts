import { Component } from '@angular/core';

import { NavController, NavParams, AlertController} from 'ionic-angular';
import { Storage } from '@ionic/storage';

import { ConfigPage } from '../configuration/configuration';

@Component({
  selector: 'page-list',
  templateUrl: 'list.html'
})
export class ListPage {
  configurations: Array<{
    id: number,
    title: string, 
    active: boolean,
    fullscreen: boolean,
    width: number,
    height: number,
    background: { r: number, g: number, b: number },
    rows: number,
    cols: number,
    zones: number[][],
    players: Array<{
      id: number,
      background:{ r: number, g: number, b: number },
      contents: Array<{
        id: number,
        title: string,
        type: string,
        load: string,
        reload: boolean,
        loop: string,
        time: number,
        repetitions: number
      }>
    }>
  }>;

  constructor(public navCtrl: NavController, public navParams: NavParams, public alertCtrl: AlertController, public storage: Storage) {
    this.configurations = [];

    storage.ready().then(() => {
      storage.get('configurations').then((data) => {
        if(data)
          this.configurations = data;
        else
          this.addConfiguration(data);
      });
    });
  }

  openConfig(event, config) {
    this.navCtrl.push(ConfigPage, {
      config: config,
      list: this
    });
  }

  deleteConfiguration(event, config) {
    if(this.configurations.length>1){
      var index = -1;
      for(let i=0; i<this.configurations.length; i++){
        if(this.configurations[i].id == config.id)
          index=i;
      }
      if (index > -1) {
        this.configurations.splice(index, 1);
        this.storage.set('configurations',this.configurations);
      }
    }
    else{
      let alert = this.alertCtrl.create({
        title: "Cannot delete all configurations",
        subTitle: "Sorry ;)",
        buttons: ["OK"]
      });
      alert.present();
    }
    this.saveConfigurations();
  }

  addConfiguration(event){
    this.configurations.push({
      id: this.configurations.length,
      title: 'Configuration ' + this.configurations.length,
      active: false,
      fullscreen: true,
      width: 1920,
      height: 1080,
      background: { r: 0, g: 0, b: 0 },
      rows: 3,
      cols: 3,
      zones: [ [ 0, 0, 0], [0, 0, 0], [0 , 0, 0] ],
      players: [ {
        id: 0,
        background:{ r: 255, g: 255, b: 255 },
        contents: [ {
          id: 0,
          title: "Content 0",
          type: "image",
          load: "logo.png",
          reload: false,
          loop: "none",
          time: 0,
          repetitions: 0
        } ] 
      } ]
    });
    this.saveConfigurations();
  }

  enableConfiguration(id){
    for(let i = 0; i < this.configurations.length; i++)
      this.configurations[i].active = (this.configurations[i].id==id);
  }

  saveConfigurations(){
    this.storage.set('configurations',this.configurations);
  }
}
