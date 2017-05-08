import { Injectable } from "@angular/core";
import { Observable } from 'rxjs/Observable';
import { Storage } from '@ionic/storage';

@Injectable()
export class DataBaseService {
	server: string;
	configurations: Array<{
	    id: number,
	    title: string, 
	    active: boolean,
	    fullscreen: boolean,
	    width: string,
	    height: string,
	    background: { r: string, g: string, b: string },
	    rows: string,
	    cols: string,
	    zones: number[][],
	    players: Array<{
	      id: number,
	      background: { r: string, g: string, b: string },
	      contents: Array<{
	        id: number,
	        title: string,
	        type: string,
	        load: string,
	        top: string,
	        bottom: string,
	        date: { year: number, month: number, day: number, hour: number, minute: number },
	        reload: boolean,
	        loop: string,
	        time: string,
	        repetitions: string,
	        qty: string
	      }>
	    }>
	  }>;

	dataBase: any;
    dataBaseObserver: any;

    constructor(public storage: Storage) {
    	this.dataBase = Observable.create(observer => {
      		this.dataBaseObserver = observer;
    	});

		storage.ready().then(() => {
	      storage.get('server').then((data) => {
	        if(data)
	          this.server = data;
	        else
	          this.server = 'http://localhost:7890';
	      	this.dataBaseObserver.next(true);
	      });

	      storage.get('configurations').then((data) => {
        	if(data)
          		this.configurations = data;
	        else
	          	this.configurations = [];
	        this.dataBaseObserver.next(true);
	      });
	    });
    }

    saveServer(newServer) {
    	this.server = newServer;
  		this.storage.set('server',this.server);
  		this.dataBaseObserver.next(true);
  	}

	saveConfigurations(newConfigurations){
		this.configurations = newConfigurations;
    	this.storage.set('configurations',this.configurations);
    	this.dataBaseObserver.next(true);
  	}
}