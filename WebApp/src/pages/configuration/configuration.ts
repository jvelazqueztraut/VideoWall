import { Component } from '@angular/core';
import { Http } from '@angular/http';

import { NavController, NavParams, AlertController, ToastController } from 'ionic-angular';

import { ContentPage } from '../content/content';

@Component({
  selector: 'page-configuration',
  templateUrl: 'configuration.html'
})
export class ConfigPage {
  selectedConfig: any;
  listPage: any;

  constructor(public navCtrl: NavController, public navParams: NavParams, public alertCtrl: AlertController, public toastCtrl: ToastController, public http: Http) {
    // If we navigated to this page, we will have an item available as a nav param
    this.selectedConfig = this.navParams.get('config');
    this.listPage = this.navParams.get('list');
  }

  addPlayer(event, config) {
    config.players.push({
      id: config.players.length,
      zones : [ 0, 0, 0, 0, 0, 0, 0, 0, 0],
      background:{ r: 255, g: 255, b: 255 },
      contents: [ {
        id: 0,
        type: "image",
        load: "logo.png",
        reload: false,
        loop: "none",
        time: 0,
        repetitions: 0
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
    player.contents.push({
      id: player.contents.length,
      type: "image",
      load: "logo.png",
      reload: false,
      loop: "none",
      time: 0,
      repetitions: 0
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

  changeZonePlayer(r,c) {
    this.selectedConfig.zones[r][c]++;
    if(this.selectedConfig.zones[r][c]>=this.selectedConfig.players.length)
      this.selectedConfig.zones[r][c]=0;
  }

  getPlayerZones(player) {
    let zones: Array<number> = [];
    for(let i =0; i < this.selectedConfig.zones.length; i++){
      if(this.selectedConfig.zones[i] == player)
        zones.push(i);
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
              this.sendConfiguration();
              this.listPage.enableConfiguration(this.selectedConfig.id);
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
    var link = 'http://localhost:7890/post';
    var data = JSON.stringify(this.selectedConfig);
    
    this.http.post(link, data).subscribe(data => {
      console.log(data.status);
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
  }
}
