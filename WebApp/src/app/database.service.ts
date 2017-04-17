import { Injectable } from "@angular/core";
import { Storage } from '@ionic/storage';

@Injectable()
export class DataBaseService {
	server: string;

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

    constructor(public storage: Storage) {
		storage.ready().then(() => {
	      storage.get('server').then((data) => {
	        if(data)
	          this.server = data;
	        else
	          this.server = 'http://localhost:7890';
	      });

	      storage.get('configurations').then((data) => {
        	if(data)
          		this.configurations = data;
	        else
	          	this.configurations = [];
	      });
	    });
    }

    saveServer(newServer) {
    	this.server = newServer;
  		this.storage.set('server',this.server);
  	}

	saveConfigurations(newConfigurations){
		this.configurations = newConfigurations;
    	this.storage.set('configurations',this.configurations);
  	}
}