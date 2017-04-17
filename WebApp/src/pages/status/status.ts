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
	framerate: number;
	players: number;

  constructor(public dataBase: DataBaseService, public http: Http) {
    dataBase.dataBase.subscribe(data => {
      this.server = dataBase.server;
    });
    this.server = dataBase.server;

    this.getStatus();
    setInterval(() => this.getStatus(), 5000);
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

  changeServer(event){
    this.dataBase.saveServer(this.server);
  }
}
