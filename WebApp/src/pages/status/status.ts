import { Component } from '@angular/core';
import { DataBaseService } from '../../app/database.service';
import { Http } from '@angular/http';

@Component({
  selector: 'page-status',
  templateUrl: 'status.html'
})
export class StatusPage {
	server: string;
	online: boolean;
	framerate: string;
	players: number;

  polling: any;

  constructor(public dataBase: DataBaseService, public http: Http) {
    dataBase.dataBase.subscribe(data => {
      this.server = dataBase.server;
    });
    this.server = dataBase.server;
  }

  ionViewDidLoad(){
    this.getStatus();
    this.polling = setInterval(() => this.getStatus(), 5000);
  }

  ionViewDidLeave(){
    clearInterval(this.polling);
  }

  getStatus() {
    var link = this.server + '/post';
    var data = "";

    this.http.post(link,data).subscribe(data => {
      console.log(data);
      if(data.ok){
        this.online = true;
        this.framerate = data.statusText;
      }
      else{
        this.online = false;
        this.framerate = data.statusText;
      }
    }, error => {
      this.online = false;
      this.framerate = "";
    });
  }

  changeServer(event){
    this.dataBase.saveServer(this.server);
  }
}
