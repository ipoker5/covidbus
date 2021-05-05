package clases;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Tipo_sensor {
	Integer idtipo_sensor;
	Float valor;
	Integer idsensor;
	
	public Tipo_sensor(@JsonProperty("idtipo_sensor")Integer idtipo_sensor, @JsonProperty("valor")Float valor, @JsonProperty("idsensor")Integer idsensor) {
		super();
		this.idtipo_sensor = idtipo_sensor;
		this.valor = valor;
		this.idsensor = idsensor;
	}

	public Integer getIdsensor() {
		return idsensor;
	}

	public void setIdsensor(Integer idsensor) {
		this.idsensor = idsensor;
	}

	public Integer getIdtipo_sensor() {
		return idtipo_sensor;
	}

	public void setIdtipo_sensor(Integer idtipo_sensor) {
		this.idtipo_sensor = idtipo_sensor;
	}

	public Float getValor() {
		return valor;
	}

	public void setValor(Float valor) {
		this.valor = valor;
	}


	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((idtipo_sensor == null) ? 0 : idtipo_sensor.hashCode());
		result = prime * result + ((valor == null) ? 0 : valor.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Tipo_sensor other = (Tipo_sensor) obj;
		if (idtipo_sensor == null) {
			if (other.idtipo_sensor != null)
				return false;
		} else if (!idtipo_sensor.equals(other.idtipo_sensor))
			return false;
		if (valor == null) {
			if (other.valor != null)
				return false;
		} else if (!valor.equals(other.valor))
			return false;
		return true;
	}
	

}
