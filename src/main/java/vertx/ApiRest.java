package vertx;


import clases.Sensor_Actuador;
import clases.Tipo_actuador;
import clases.Tipo_gps;
import clases.Tipo_sensor;
import clases.Dispositivo;
import clases.Usuario;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Promise;
import io.vertx.core.json.Json;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.mysqlclient.MySQLConnectOptions;
import io.vertx.mysqlclient.MySQLPool;
import io.vertx.sqlclient.PoolOptions;
import io.vertx.sqlclient.Row;
import io.vertx.sqlclient.RowSet;
import io.vertx.sqlclient.Tuple;

public class ApiRest extends AbstractVerticle{
	
	//Sirve despues para hacer conexiones get,put..
	private MySQLPool mySqlClient;
	@Override
	public void start(Promise<Void> startPromise) {
		
		
		MySQLConnectOptions mySQLConnectOptions = new MySQLConnectOptions().setPort(3306).setHost("localhost")
				.setDatabase("covidbus").setUser("root").setPassword("ivan1998");
		PoolOptions poolOptions = new PoolOptions().setMaxSize(5);// numero maximo de conexiones
		
		mySqlClient = MySQLPool.pool(vertx, mySQLConnectOptions, poolOptions);
		
		Router router = Router.router(vertx); // Permite canalizar las peticiones
		router.route().handler(BodyHandler.create());
		//Creacion de un servidor http, recibe por parametro el puerto, el resultado
		vertx.createHttpServer().requestHandler(router::handle).listen(8080, result -> {
			if (result.succeeded()) {
				startPromise.complete();
			}else {
				startPromise.fail(result.cause());
			}
		});
		// Definimos la rutas que se le pasan al servido http
		// ":idusuario" es un parametro que se le pasa a la funcion que se le llama en handler
		// no tener dos metodos put,get... con el mismo recurso por que el router no sabria por donde tirar
		router.get("/api/usuario/:idusuario").handler(this::getUsuario);
		router.put("/api/PutUsuario/:idusuario").handler(this::PutUsuario);
		router.delete("/api/EliminarUsuario/:idusuario").handler(this::DeleteUsuario);
		router.post("/api/PostUsuario/").handler(this::postUsuario);
		
		router.get("/api/dispositivo/:iddispositivo").handler(this::getDipositivo);
		router.get("/api/dispositivosUsuarios/").handler(this::getDipositivosUsuarios);
		router.get("/api/dispositivosUsuarios/:idusuario").handler(this::getDipositivoUsuario);
		router.put("/api/PutDispositivo/:iddispositivo").handler(this::PutDispositivo);
		router.delete("/api/EliminarDispositivo/:iddispositivo").handler(this::DeleteDispositivo);
		router.post("/api/PostDispositivo/").handler(this::postDispositivo);
		
		
		router.get("/api/sensor/:idsensor").handler(this::getSensor);
		router.get("/api/tipoSensor/:idtipo_sensor").handler(this::getTipoSensor);
		router.get("/api/tipoSensorGPS/:idtipo_gps").handler(this::getSensorGPS);
		
		router.get("/api/actuador/:idactuador").handler(this::getActuador);
		router.get("/api/tipoActuador/:idtipo_actuador").handler(this::getTipoActuador);
		
		
		getAll();
		
		
		
	}
	private void getAll() {
		mySqlClient.query("SELECT * FROM covidbus.usuario;", res -> {
			if (res.succeeded()) {
				// Get the result set
				RowSet<Row> resultSet = res.result();
				//System.out.println(resultSet.size());
				JsonArray result = new JsonArray();
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Usuario(elem.getInteger("idusuario"), elem.getString("nombre"),
							elem.getString("contrase�a"), elem.getString("ciudad"))));
				}
				System.out.println(result.encodePrettily());
			} else {
				System.out.println("Error: " + res.cause().getLocalizedMessage());
			}
		});
	}
	private void getUsuario(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idusuario=Integer.parseInt(routingContext.request().getParam("idusuario"));
		
		mySqlClient.query("SELECT * FROM covidbus.usuario WHERE idusuario = '" + idusuario + "'", res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Usuario(elem.getInteger("idusuario"),
							elem.getString("nombre"),
							elem.getString("contrase�a"),
							elem.getString("ciudad"))));
					
				}
				routingContext.response().putHeader("content-type", "application/json").setStatusCode(200)
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().putHeader("content-type", "application/json").setStatusCode(401)
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
		});
	}
	private void getDipositivo(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer iddispositivo=Integer.parseInt(routingContext.request().getParam("iddispositivo"));
		
		mySqlClient.query("SELECT * FROM covidbus.dispositivo WHERE iddispositivo = '" + iddispositivo + "'", res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Dispositivo(elem.getInteger("iddispositivo"),
							elem.getString("autobus"),
							elem.getInteger("idusuario"))));
					
				}
				routingContext.response().putHeader("content-type", "application/json").setStatusCode(200)
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().putHeader("content-type", "application/json").setStatusCode(401)
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
		});
	}
	private void getDipositivosUsuarios(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		
		mySqlClient.query("SELECT * FROM covidbus.dispositivo WHERE idusuario;", res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Dispositivo(elem.getInteger("iddispositivo"),
							elem.getString("autobus"),
							elem.getInteger("idusuario"))));
				}
				routingContext.response().putHeader("content-type", "application/json").setStatusCode(200)
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().putHeader("content-type", "application/json").setStatusCode(401)
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
		});
	}
	private void getDipositivoUsuario(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idusuario=Integer.parseInt(routingContext.request().getParam("idusuario"));
		
		mySqlClient.query("SELECT * FROM covidbus.dispositivo WHERE idusuario = '" + idusuario + "'",res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Dispositivo(elem.getInteger("iddispositivo"),
							elem.getString("autobus"),
							elem.getInteger("idusuario"))));
				}
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
				.end(result.encodePrettily());
			}else {
				routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
			}
			});
	}
	private void getSensor(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idsensor=Integer.parseInt(routingContext.request().getParam("idsensor"));
		
		mySqlClient.query("SELECT * FROM covidbus.sensor WHERE idsensor = '" + idsensor + "'",res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Sensor_Actuador(elem.getInteger("idsensor"),
							elem.getString("tipo"),
							elem.getString("nombre"),
							elem.getInteger("iddispositivo"))));
				}
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
			});
	}
	private void getActuador(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idactuador=Integer.parseInt(routingContext.request().getParam("idactuador"));
		
		mySqlClient.query("SELECT * FROM covidbus.actuador WHERE idactuador = '" + idactuador + "'",res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Sensor_Actuador(elem.getInteger("idactuador"),
							elem.getString("tipo"),
							elem.getString("nombre"),
							elem.getInteger("iddispositivo"))));
				}
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
			});
	}
	private void getTipoActuador(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idtipo_actuador=Integer.parseInt(routingContext.request().getParam("idtipo_actuador"));
		
		mySqlClient.query("SELECT * FROM covidbus.tipo_actuador WHERE idtipo_actuador = '" + idtipo_actuador + "'",res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Tipo_actuador(elem.getInteger("idtipo_actuador"),
							elem.getInteger("valor"),
							elem.getInteger("modo"),
							elem.getInteger("idactuador"))));
				}
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
			});
	}
	private void getTipoSensor(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idtipo_sensor=Integer.parseInt(routingContext.request().getParam("idtipo_sensor"));
		
		mySqlClient.query("SELECT * FROM covidbus.tipo_sensor WHERE idtipo_sensor = '" + idtipo_sensor + "'",res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Tipo_sensor(elem.getInteger("idtipo_sensor"),
							elem.getFloat("valor"),
							elem.getInteger("idsensor"))));
				}
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
			});
	}
	private void getSensorGPS(RoutingContext routingContext) {
		// routing da un contenido en formato string por lo que hay que parsearlo
		Integer idtipo_gps=Integer.parseInt(routingContext.request().getParam("idtipo_gps"));
		
		mySqlClient.query("SELECT * FROM covidbus.tipo_gps WHERE idtipo_gps = '" + idtipo_gps + "'",res -> {
			if (res.succeeded()) {	
				RowSet<Row> resultSet = res.result();
				JsonArray result = new JsonArray();
				
				for (Row elem : resultSet) {
					result.add(JsonObject.mapFrom(new Tipo_gps(elem.getInteger("idtipo_gps"),
							elem.getFloat("x"),
							elem.getFloat("y"),
							elem.getLong("timestamp"))));
				}
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
				.end(result.encodePrettily());
				System.out.println(result.encodePrettily());
			}else {
				routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
				.end((JsonObject.mapFrom(res.cause()).encodePrettily()));
				System.out.println("Error"+res.cause().getLocalizedMessage());
			}
			});
	}
	private void PutUsuario(RoutingContext routingContext) { //Actualiza un usuario
		Usuario usuario = Json.decodeValue(routingContext.getBodyAsString(), Usuario.class);
		mySqlClient.preparedQuery(
				"UPDATE usuario SET nombre = ?, contrase�a = ?, ciudad = ? WHERE idusuario = ?",
				Tuple.of(usuario.getNombre(), usuario.getContrase�a(), usuario.getCiudad(), routingContext.request().getParam("idusuario")),
				handler -> {
					if (handler.succeeded()) {
						routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
								.end(JsonObject.mapFrom(usuario).encodePrettily());
					} else {
						routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
								.end((JsonObject.mapFrom(handler.cause()).encodePrettily()));
					}
				});
	}
	private void PutDispositivo(RoutingContext routingContext) { //Actualiza un usuario
		Dispositivo dispositivo = Json.decodeValue(routingContext.getBodyAsString(), Dispositivo.class);
		mySqlClient.preparedQuery(
				"UPDATE dispositivo SET autobus = ?, idusuario = ? WHERE iddispositivo = ?",
				Tuple.of(dispositivo.getAutobus(), dispositivo.getIdusuario(), routingContext.request().getParam("iddispositivo")),
				handler -> {
					if (handler.succeeded()) {
						routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
								.end(JsonObject.mapFrom(dispositivo).encodePrettily());
					} else {
						routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
								.end((JsonObject.mapFrom(handler.cause()).encodePrettily()));
					}
				});
	}
	private void DeleteUsuario(RoutingContext routingContext) {
		Integer idusuario=Integer.parseInt(routingContext.request().getParam("idusuario"));
		mySqlClient.query("DELETE FROM covidbus.usuario WHERE idusuario =  " + idusuario,handler -> {		
			if (handler.succeeded()) {						
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json").end("Usuario borrado correctamente");
			}else {
					routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
					.end((JsonObject.mapFrom(handler.cause()).encodePrettily()));
			}
		});
	}
	private void DeleteDispositivo(RoutingContext routingContext) {
		Integer iddispositivo=Integer.parseInt(routingContext.request().getParam("iddispositivo"));
		mySqlClient.query("DELETE FROM covidbus.dispositivo WHERE iddispositivo =  " + iddispositivo,handler -> {		
			if (handler.succeeded()) {						
				routingContext.response().setStatusCode(200).putHeader("content-type", "application/json").end("Usuario borrado correctamente");
			}else {
					routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
					.end((JsonObject.mapFrom(handler.cause()).encodePrettily()));
			}
		});
	}
	private void postUsuario(RoutingContext routingContext){
		Usuario usuario = Json.decodeValue(routingContext.getBodyAsString(), Usuario.class);	
		mySqlClient.preparedQuery("INSERT INTO usuario (idusuario, nombre, contrase�a, ciudad) VALUES (?,?,?,?)",
				Tuple.of(usuario.getIdusuario(), usuario.getNombre(),
						usuario.getContrase�a(), usuario.getCiudad()),handler -> {	
				if (handler.succeeded()) {
					routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
							.end(JsonObject.mapFrom(usuario).encodePrettily());
					}else {
						routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
						.end((JsonObject.mapFrom(handler.cause()).encodePrettily()));
				}
			});
		
	}
	private void postDispositivo(RoutingContext routingContext){
		Dispositivo dispositivo = Json.decodeValue(routingContext.getBodyAsString(), Dispositivo.class);	
		mySqlClient.preparedQuery("INSERT INTO dispositivo (iddispositivo, autobus, idusuario) VALUES (?,?,?)",
				Tuple.of(dispositivo.getIddispositivo(), dispositivo.getAutobus(),
						dispositivo.getIdusuario()),handler -> {	
				if (handler.succeeded()) {
					routingContext.response().setStatusCode(200).putHeader("content-type", "application/json")
							.end(JsonObject.mapFrom(dispositivo).encodePrettily());
					}else {
						routingContext.response().setStatusCode(401).putHeader("content-type", "application/json")
						.end((JsonObject.mapFrom(handler.cause()).encodePrettily()));
				}
			});
		
	}
}
