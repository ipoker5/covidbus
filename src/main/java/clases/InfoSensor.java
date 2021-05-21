package clases;

import com.fasterxml.jackson.annotation.JsonProperty;

public class InfoSensor {
	Integer idsensor;
	String tipo;
	String nombre;
	Float last_value1;
	Float last_value2;
	Integer iddispositivo;
	
	public InfoSensor(@JsonProperty("idsensor")Integer idsensor,@JsonProperty("tipo")String tipo,@JsonProperty("nombre")String nombre,
			@JsonProperty("last_value1")Float last_value1,@JsonProperty("last_value2")Float last_value2,@JsonProperty("iddispositivo")Integer iddispositivo) {
		super();
		this.idsensor = idsensor;
		this.tipo = tipo;
		this.nombre = nombre;
		this.last_value1=last_value1;
		this.last_value2=last_value2;
		this.iddispositivo = iddispositivo;
	}

	public Integer getIdsensor() {
		return idsensor;
	}

	public void setIdsensor(Integer idsensor) {
		this.idsensor = idsensor;
	}

	public String getTipo() {
		return tipo;
	}

	public void setTipo(String tipo) {
		this.tipo = tipo;
	}

	public String getNombre() {
		return nombre;
	}

	public void setNombre(String nombre) {
		this.nombre = nombre;
	}

	public Float getLast_value1() {
		return last_value1;
	}

	public void setLast_value1(Float last_value1) {
		this.last_value1 = last_value1;
	}

	public Float getLast_value2() {
		return last_value2;
	}

	public void setLast_value2(Float last_value2) {
		this.last_value2 = last_value2;
	}

	public Integer getIddispositivo() {
		return iddispositivo;
	}

	public void setIddispositivo(Integer iddispositivo) {
		this.iddispositivo = iddispositivo;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((iddispositivo == null) ? 0 : iddispositivo.hashCode());
		result = prime * result + ((idsensor == null) ? 0 : idsensor.hashCode());
		result = prime * result + ((last_value1 == null) ? 0 : last_value1.hashCode());
		result = prime * result + ((last_value2 == null) ? 0 : last_value2.hashCode());
		result = prime * result + ((nombre == null) ? 0 : nombre.hashCode());
		result = prime * result + ((tipo == null) ? 0 : tipo.hashCode());
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
		InfoSensor other = (InfoSensor) obj;
		if (iddispositivo == null) {
			if (other.iddispositivo != null)
				return false;
		} else if (!iddispositivo.equals(other.iddispositivo))
			return false;
		if (idsensor == null) {
			if (other.idsensor != null)
				return false;
		} else if (!idsensor.equals(other.idsensor))
			return false;
		if (last_value1 == null) {
			if (other.last_value1 != null)
				return false;
		} else if (!last_value1.equals(other.last_value1))
			return false;
		if (last_value2 == null) {
			if (other.last_value2 != null)
				return false;
		} else if (!last_value2.equals(other.last_value2))
			return false;
		if (nombre == null) {
			if (other.nombre != null)
				return false;
		} else if (!nombre.equals(other.nombre))
			return false;
		if (tipo == null) {
			if (other.tipo != null)
				return false;
		} else if (!tipo.equals(other.tipo))
			return false;
		return true;
	}

	

}
