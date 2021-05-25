package clases;

import com.fasterxml.jackson.annotation.JsonProperty;


public class Dispositivo {
	
	Integer iddispositivo;
	String autobus;
	Integer idusuario;

	public Dispositivo( @JsonProperty("iddispositivo")Integer iddispositivo, 
			@JsonProperty("autobus")String autobus, @JsonProperty("idusuario")Integer idusuario) {
		super();
		this.iddispositivo = iddispositivo;
		this.autobus = autobus;
		this.idusuario = idusuario;
	}

	public Integer getIddispositivo() {
		return iddispositivo;
	}

	public void setIddispositivo(Integer iddispositivo) {
		this.iddispositivo = iddispositivo;
	}

	public String getAutobus() {
		return autobus;
	}

	public void setAutobus(String autobus) {
		this.autobus = autobus;
	}

	public Integer getIdusuario() {
		return idusuario;
	}

	public void setIdusuario(Integer idususario) {
		this.idusuario = idususario;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((autobus == null) ? 0 : autobus.hashCode());
		result = prime * result + ((iddispositivo == null) ? 0 : iddispositivo.hashCode());
		result = prime * result + ((idusuario == null) ? 0 : idusuario.hashCode());
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
		Dispositivo other = (Dispositivo) obj;
		if (autobus == null) {
			if (other.autobus != null)
				return false;
		} else if (!autobus.equals(other.autobus))
			return false;
		if (iddispositivo == null) {
			if (other.iddispositivo != null)
				return false;
		} else if (!iddispositivo.equals(other.iddispositivo))
			return false;
		if (idusuario == null) {
			if (other.idusuario != null)
				return false;
		} else if (!idusuario.equals(other.idusuario))
			return false;
		return true;
	}



}