import { Component } from '@angular/core';
import { Http } from '@angular/http';

import { NavController, NavParams, AlertController, ToastController, PopoverController } from 'ionic-angular';

import { DataBaseService } from '../../app/database.service';
import { ContentPage } from '../content/content';

import { ColorPopover } from '../color/color';

@Component({
  selector: 'page-configuration',
  templateUrl: 'configuration.html'
})
export class ConfigPage {
  selectedConfig: any;
  configurations: any;

  constructor(public navCtrl: NavController, public navParams: NavParams, public alertCtrl: AlertController, public toastCtrl: ToastController, public popoverCtrl: PopoverController, public dataBase: DataBaseService, public http: Http) {
    // If we navigated to this page, we will have an item available as a nav param
    this.selectedConfig = this.navParams.get('selected');
    this.configurations = this.navParams.get('all');
  }

  addPlayer(event, config) {
    let now = new Date();
    let id = 0;
    let used = false;
    do{
      if(used) id++;
      used = false;
      for(let i=0; i<config.players.length; i++){
        if(config.players[i].id == id)
          used=true;
      }
    }while(used);
    config.players.push({
      id: id,
      zones : [ 0, 0, 0, 0, 0, 0, 0, 0, 0],
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
    });
  }

  deletePlayer(event, player) {
    if(this.selectedConfig.players.length>1){
      var index = -1;
      for(let i=0; i<this.selectedConfig.players.length; i++){
        if(this.selectedConfig.players[i].id == player.id)
          index=i;
      }
      if (index > -1) {
        this.selectedConfig.players.splice(index, 1);
      }
    }
    else{
      let alert = this.alertCtrl.create({
        title: "Cannot delete all players from configuration",
        subTitle: "Sorry ;)",
        buttons: ["OK"]
      });
      alert.present();
    }
  }

  openContent(event,content) {
    this.navCtrl.push(ContentPage,{
      content: content
    });
  }

  addContent(event, player) {
    let now = new Date();
    let id = 0;
    let used = false;
    do{
      if(used) id++;
      used = false;
      for(let i=0; i<player.contents.length; i++){
        if(player.contents[i].id == id)
          used=true;
      }
    }while(used);
    player.contents.push({
      id: id,
      title: 'Content ' + id,
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
    });
    this.navCtrl.push(ContentPage,{
      content: player.contents[player.contents.length-1]
    });
  }

  deleteContent(event, player, content) {
    if(player.contents.length>1){
      var index = -1;
      for(let i=0; i<player.contents.length; i++){
        if(player.contents[i].id == content.id)
          index=i;
      }
      if (index > -1) {
        player.contents.splice(index, 1);
      }
    }
    else{
      let alert = this.alertCtrl.create({
        title: "Cannot delete all content from player",
        subTitle: "Sorry ;)",
        buttons: ["OK"]
      });
      alert.present();
    }
  }

  changeZonePlayer(c,r) {
    for(let i=0; i<this.selectedConfig.players.length; i++){
      if(this.selectedConfig.players[i].id == this.selectedConfig.zones[r][c]){
        if((i+1)<this.selectedConfig.players.length){
          this.selectedConfig.zones[r][c] = this.selectedConfig.players[i+1].id;
          return;
        }
      }
    }
    this.selectedConfig.zones[r][c] = this.selectedConfig.players[0].id;
  }

  getPlayerZones(player) {
    let zones: Array<number> = [];
    for(let x =0; x < this.selectedConfig.zones.length; x++){
      for(let y =0; y < this.selectedConfig.zones[x].length; y++){
        if(this.selectedConfig.zones[x][y] == player)
          zones.push(y+x*this.selectedConfig.rows);
      }
    }
    return zones;
  }

  setActive(event) {
    if(event){
      let confirm = this.alertCtrl.create({
        title: "Are you sure you want to enable this configuration?",
        message: "Doing this will replace the current configuration",
        buttons: [
          {
            text: "Cancel",
            handler: () => {
              this.selectedConfig.active = false;
            }
          },
          {
            text: "OK",
            handler: () => {
              this.selectedConfig.active = true;
              this.enableConfiguration(this.selectedConfig.id);
              this.sendConfiguration();
            }
          }
        ]
      });
      confirm.present();
    }
    else{
      let alert = this.alertCtrl.create({
        title: "You cant disable the current configuration",
        subTitle: "To replace the configuration you should enable another one",
        buttons: [ {
          text: "OK",
          handler: () => {
            this.selectedConfig.active = true;
          }
        } ]
      });
      alert.present();
    }
  }

  sendConfiguration() {
    var link = this.dataBase.server + '/post';
    var data = JSON.stringify(this.selectedConfig);
    
    this.http.post(link, data).subscribe(data => {
      if(data.ok){
        let toast = this.toastCtrl.create({
          message: 'Configuration enabled successfully',
          duration: 2000,
          position: 'top'
        });
        toast.present();
        this.selectedConfig.active = true;
      }
      else{
        let toast = this.toastCtrl.create({
          message: 'There was an error sending the configuration',
          showCloseButton: true,
          closeButtonText: 'Ok',
          duration: 5000,
          position: 'top'
        });
        toast.present();
        this.selectedConfig.active = false;
      }
    }, error => {
      let alert = this.alertCtrl.create({
        title: "Error",
        subTitle: "Server seems to be offline. Please restart the VideoWall main app.",
        buttons: ["OK"]
      });
      alert.present();
      this.selectedConfig.active = false;
    });

    this.dataBase.saveConfigurations(this.configurations);
  }

  enableConfiguration(id){
      for(let i = 0; i < this.configurations.length; i++)
        this.configurations[i].active = (this.configurations[i].id==id);
  }

  getColor(color){
    return 'rgb('+color.r+','+color.g+','+color.b+')';
  }

  chooseColor(event,c) {
    let popover = this.popoverCtrl.create(ColorPopover, {
      color: c
    });

    popover.present({
      ev: event
    });
  }

}
