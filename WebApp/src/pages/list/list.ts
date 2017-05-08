import { Component } from '@angular/core';

import { NavController, NavParams, AlertController} from 'ionic-angular';

import { DataBaseService } from '../../app/database.service';
import { ConfigPage } from '../configuration/configuration';

@Component({
  selector: 'page-list',
  templateUrl: 'list.html'
})
export class ListPage {
  configurations: any;

  constructor(public navCtrl: NavController, public navParams: NavParams, public alertCtrl: AlertController, public dataBase: DataBaseService) {
    this.configurations = dataBase.configurations;
    dataBase.dataBase.subscribe(data => {
      this.configurations = dataBase.configurations;
    });
  }

  openConfig(event, config) {
    this.navCtrl.push(ConfigPage, {
      selected: config,
      all: this.configurations
    });
  }

  deleteConfiguration(event, config) {
    var index = -1;
    for(let i=0; i<this.configurations.length; i++){
      if(this.configurations[i].id == config.id)
        index=i;
    }
    if (index > -1) {
      this.configurations.splice(index, 1);
    }
    this.dataBase.saveConfigurations(this.configurations);
  }

  addConfiguration(event){
    let now = new Date();
    let id = 0;
    let used = false;
    do{
      if(used) id++;
      used = false;
      for(let i=0; i<this.configurations.length; i++){
        if(this.configurations[i].id == id)
          used=true;
      }
    }while(used);
    this.configurations.push({
      id: id,
      title: 'Configuration ' + id,
      active: false,
      fullscreen: true,
      width: "1920",
      height: "1080",
      background: { r: "0", g: "0", b: "0" },
      rows: "3",
      cols: "3",
      zones: [ [ 0, 0, 0], [0, 0, 0], [0 , 0, 0] ],
      players: [ {
        id: 0,
        background: { r: "255", g: "255", b: "255" },
        contents: [ {
          id: 0,
          title: "Content 0",
          type: "image",
          load: "logo.png",
          top: "",
          bottom: "",
          date: { year: now.getFullYear(), month: now.getMonth(), day: now.getDate() , hour: now.getHours(), minute: 0 },
          reload: false,
          loop: "none",
          time: "0",
          repetitions: "0",
          qty: "0"
        } ] 
      } ]
    });
    this.dataBase.saveConfigurations(this.configurations);
  }
}
