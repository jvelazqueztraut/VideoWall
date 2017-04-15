import { Component } from '@angular/core';
import { Storage } from '@ionic/storage';
import { Http } from '@angular/http';

@Component({
  selector: 'page-status',
  templateUrl: 'status.html'
})
export class StatusPage {
	server: string;
	online: boolean;
	framerate: number;
	players: number;

  constructor(public storage: Storage, public http: Http) {
  	storage.ready().then(() => {
      storage.get('server').then((data) => {
        if(data)
          this.server = data;
        else
          this.server = 'http://localhost:7890';
      	this.getStatus();
      	setInterval(() => this.getStatus(), 5000);
      });
    });
  }

  getStatus() {
    var link = this.server + '/post';
    var data = "";

    this.http.post(link,data).subscribe(data => {
      console.log(data);
      if(data.ok){
        this.online = true;
        this.framerate = 30;
      }
      else{
        this.online = false;
        this.framerate = 0;
      }
    }, error => {
      this.online = false;
      this.framerate = 0;
    });
  }

  saveServer() {
  	this.storage.set('server',this.server);
  }
}
